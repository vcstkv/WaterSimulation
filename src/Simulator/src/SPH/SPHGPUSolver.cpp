#define _USE_MATH_DEFINES
#include "SPH/SPHGPUSolver.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "cmath"
#include <algorithm>

#include "../data/Shaders/ComputeHeaders/SsboBufferBindings.comp"
#include "../data/Shaders/ComputeHeaders/UniformLocations.comp"
#include "../data/Shaders/ParallelSort/ParallelSortConstants.comp"

typedef unsigned int uint;

#define WORK_GROUP_SIZE 256

SPHGPUSolver::SPHGPUSolver(SPHFluidParams &params, BoundaryBox &box, std::vector<SPHParticle> &particles)
{
	particlesCount = particles.size();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW init error");
	}

	accumulateForces.AddShader(GL_COMPUTE_SHADER, accumulateForcesPath);
	accumulateForces.Link();

	densityAndPressure.AddShader(GL_COMPUTE_SHADER, densityAndPressurePath);
	densityAndPressure.Link();

	indexClear.AddShader(GL_COMPUTE_SHADER, indexClearPath);
	indexClear.Link();

	indexUpdate.AddShader(GL_COMPUTE_SHADER, indexUpdatePath);
	indexUpdate.Link();
	
	leapfrogIntegration.AddShader(GL_COMPUTE_SHADER, leapfrogIntegrationPath);
	leapfrogIntegration.Link();

	boundaries.AddShader(GL_COMPUTE_SHADER, boundariesPath);
	boundaries.Link();

	render.AddShader(GL_VERTEX_SHADER, renderVertexShaderPath);
	render.AddShader(GL_GEOMETRY_SHADER, renderGeometryShaderPath);
	render.AddShader(GL_FRAGMENT_SHADER, renderFragmentShaderPath);
	render.Link();

	// take a data structure that needs to be sorted by a value (must be unsigned int for radix 
	// sort to work) and put it into an intermediate structure that has the value and the index 
	// of the original data structure in the OriginalDataBuffer
	GLuint sid = 0;
	sid = origDataToInterData.CreateCompositeShader(GL_COMPUTE_SHADER);
	origDataToInterData.AddPartialShaderFile(sid, "ComputeHeaders/Version.comp");
	origDataToInterData.AddPartialShaderFile(sid, "ComputeHeaders/SsboBufferBindings.comp");
	origDataToInterData.AddPartialShaderFile(sid, "ComputeHeaders/UniformLocations.comp");
	origDataToInterData.AddPartialShaderFile(sid, "OriginalDataBuffer.comp");
	origDataToInterData.AddPartialShaderFile(sid, "ParallelSort/ParallelSortConstants.comp");
	origDataToInterData.AddPartialShaderFile(sid, "ParallelSort/IntermediateSortBuffers.comp");
	origDataToInterData.AddPartialShaderFile(sid, "ParallelSort/OriginalDataToIntermediateData.comp");
	origDataToInterData.CompileCompositeShader(sid);
	origDataToInterData.Link();

	// on each loop in Sort(), pluck out a single bit and add it to the 
	// PrefixScanBuffer::PrefixSumsWithinGroup array
	sid = bitForPrefixScan.CreateCompositeShader(GL_COMPUTE_SHADER);
	bitForPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/Version.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/SsboBufferBindings.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/UniformLocations.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ParallelSort/ParallelSortConstants.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ParallelSort/IntermediateSortBuffers.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ParallelSort/PrefixScanBuffer.comp");
	bitForPrefixScan.AddPartialShaderFile(sid, "ParallelSort/GetBitForPrefixScan.comp");
	bitForPrefixScan.CompileCompositeShader(sid);
	bitForPrefixScan.Link();

	// run the prefix scan over PrefixScanBuffer::PrefixSumsWithinGroup, and after that run the 
	// scan again over PrefixScanBuffer::PrefixSumsByGroup
	sid = parallelPrefixScan.CreateCompositeShader(GL_COMPUTE_SHADER);
	parallelPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/Version.comp");
	parallelPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/UniformLocations.comp");
	parallelPrefixScan.AddPartialShaderFile(sid, "ComputeHeaders/SsboBufferBindings.comp");
	parallelPrefixScan.AddPartialShaderFile(sid, "ParallelSort/ParallelSortConstants.comp");
	parallelPrefixScan.AddPartialShaderFile(sid, "ParallelSort/PrefixScanBuffer.comp");
	parallelPrefixScan.AddPartialShaderFile(sid, "ParallelSort/ParallelPrefixScan.comp");
	parallelPrefixScan.CompileCompositeShader(sid);
	parallelPrefixScan.Link();

	// and finally sort the "read" array from IntermediateSortBuffers into the "write" array
	sid = sortInterData.CreateCompositeShader(GL_COMPUTE_SHADER);
	sortInterData.AddPartialShaderFile(sid, "ComputeHeaders/Version.comp");
	sortInterData.AddPartialShaderFile(sid, "ComputeHeaders/UniformLocations.comp");
	sortInterData.AddPartialShaderFile(sid, "ComputeHeaders/SsboBufferBindings.comp");
	sortInterData.AddPartialShaderFile(sid, "ParallelSort/ParallelSortConstants.comp");
	sortInterData.AddPartialShaderFile(sid, "ParallelSort/PrefixScanBuffer.comp");
	sortInterData.AddPartialShaderFile(sid, "ParallelSort/IntermediateSortBuffers.comp");
	sortInterData.AddPartialShaderFile(sid, "ParallelSort/SortIntermediateData.comp");
	sortInterData.CompileCompositeShader(sid);
	sortInterData.Link();

	// after the loop, sort the original data according to the sorted intermediate data
	sid = sortOrigData.CreateCompositeShader(GL_COMPUTE_SHADER);
	sortOrigData.AddPartialShaderFile(sid, "ComputeHeaders/Version.comp");
	sortOrigData.AddPartialShaderFile(sid, "ComputeHeaders/SsboBufferBindings.comp");
	sortOrigData.AddPartialShaderFile(sid, "ComputeHeaders/UniformLocations.comp");
	sortOrigData.AddPartialShaderFile(sid, "OriginalDataBuffer.comp");
	sortOrigData.AddPartialShaderFile(sid, "ParallelSort/ParallelSortConstants.comp");
	sortOrigData.AddPartialShaderFile(sid, "ParallelSort/IntermediateSortBuffers.comp");
	sortOrigData.AddPartialShaderFile(sid, "ParallelSort/SortOriginalData.comp");
	sortOrigData.CompileCompositeShader(sid);
	sortOrigData.Link();

	UpdateParams(params, box);

	glGenBuffers(1, &particlesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SPHParticle) * particles.size(), particles.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &indexBuffer);
	std::vector<SPHParticleIndex> indexTable(params.bucketsCountX * params.bucketsCountY, SPHParticleIndex(params.particlesCount));
	for (int i = 0, local; i < params.bucketsCountY; i++)
	{
		for (int j = 0; j < params.bucketsCountX; j++)
		{
			local = 0;

			if (i - 1 >= 0 && j - 1 >= 0)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i - 1) * params.bucketsCountX + (j - 1);
			}

			if (i - 1 >= 0)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i - 1) * params.bucketsCountX + (j - 0);
			}

			if (i - 1 >= 0 && j + 1 < params.bucketsCountX)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i - 1) * params.bucketsCountX + (j + 1);
			}

			if (j - 1 >= 0)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i - 0) * params.bucketsCountX + (j - 1);
			}

			indexTable[i * params.bucketsCountX + j].n[local++] = (i - 0) * params.bucketsCountX + (j - 0);

			if (j + 1 < params.bucketsCountX)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i - 0) * params.bucketsCountX + (j + 1);
			}

			if (i + 1 < params.bucketsCountY && j - 1 >= 0)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i + 1) * params.bucketsCountX + (j - 1);
			}

			if (i + 1 < params.bucketsCountY)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i + 1) * params.bucketsCountX + (j - 0);
			}

			if (i + 1 < params.bucketsCountY && j + 1 < params.bucketsCountX)
			{
				indexTable[i * params.bucketsCountX + j].n[local++] = (i + 1) * params.bucketsCountX + (j + 1);
			}
		}
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SPHParticleIndex) * indexTable.size(), indexTable.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, particlesBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SPHParticle), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// the size of the OriginalDataBuffer is needed by these shaders, and it is known (as 
	// per my design) only by the OriginalDataSsbo object
	origDataToInterData.Enable();
	glUniform1ui(UNIFORM_LOCATION_ORIGINAL_DATA_BUFFER_SIZE, particles.size());
	origDataToInterData.Disable();

	sortOrigData.Enable();
	glUniform1ui(UNIFORM_LOCATION_ORIGINAL_DATA_BUFFER_SIZE, particles.size());
	sortOrigData.Disable();

	glGenBuffers(1, &origDataCopyBuffer);
	std::vector<SPHParticle> origDataCopy(particles.size());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ORIGINAL_DATA_COPY_BUFFER_BINDING, origDataCopyBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, origDataCopyBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, origDataCopy.size() * sizeof(SPHParticle), origDataCopy.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// the PrefixScanBuffer is used in three shaders
	prefixSumBufferSize = (particles.size() / ITEMS_PER_WORK_GROUP);
	prefixSumBufferSize += (particles.size() % ITEMS_PER_WORK_GROUP == 0) ? 0 : 1;
	prefixSumBufferSize *= ITEMS_PER_WORK_GROUP;

	glGenBuffers(1, &prefixSumBuffer);
	std::vector<unsigned int> prefixSum(prefixSumBufferSize + 1 + ITEMS_PER_WORK_GROUP);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PREFIX_SCAN_BUFFER_BINDING, prefixSumBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, prefixSumBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, prefixSum.size() * sizeof(unsigned int), prefixSum.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	bitForPrefixScan.Enable();
	glUniform1ui(UNIFORM_LOCATION_ALL_PREFIX_SUMS_SIZE, prefixSumBufferSize);
	bitForPrefixScan.Disable();

	parallelPrefixScan.Enable();
	glUniform1ui(UNIFORM_LOCATION_ALL_PREFIX_SUMS_SIZE, prefixSumBufferSize);
	parallelPrefixScan.Disable();

	sortInterData.Enable();
	glUniform1ui(UNIFORM_LOCATION_ALL_PREFIX_SUMS_SIZE, prefixSumBufferSize);
	sortInterData.Disable();

	// see explanation in the PrefixSumSsbo constructor for why there are likely more entries in 
	// PrefixScanBuffer::PrefixSumsWithinGroup than the requested number of items that need sorting
	unsigned int numEntriesInPrefixSumBuffer = prefixSumBufferSize * 2;

	glGenBuffers(1, &interDataBuffer);
	std::vector<SPHParticleSortIntermediateData> interData(numEntriesInPrefixSumBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, INTERMEDIATE_SORT_BUFFERS_BINDING, interDataBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, interDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, interData.size() * sizeof(SPHParticleSortIntermediateData), interData.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	origDataToInterData.Enable();
	glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_HALF_SIZE, prefixSumBufferSize);
	origDataToInterData.Disable();

	bitForPrefixScan.Enable();
	glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_HALF_SIZE, prefixSumBufferSize);
	bitForPrefixScan.Disable();

	sortInterData.Enable();
	glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_HALF_SIZE, prefixSumBufferSize);
	sortInterData.Disable();
}

SPHGPUSolver::~SPHGPUSolver()
{
	glDeleteBuffers(1, &particlesBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void SPHGPUSolver::SortParticles()
{
	// Note: See the explanation at the top of PrefixSumsSsbo.cpp for calculation explanation.
	unsigned int numItemsInPrefixScanBuffer = prefixSumBufferSize;

	// begin
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlesBuffer);
	// for ParallelPrefixScan.comp, which works on 2 items per thread
	int numWorkGroupsXByItemsPerWorkGroup = numItemsInPrefixScanBuffer / ITEMS_PER_WORK_GROUP;
	int remainder = numItemsInPrefixScanBuffer % ITEMS_PER_WORK_GROUP;
	numWorkGroupsXByItemsPerWorkGroup += (remainder == 0) ? 0 : 1;

	// for other shaders, which work on 1 item per thread
	int numWorkGroupsXByWorkGroupSize = numItemsInPrefixScanBuffer / PARALLEL_SORT_WORK_GROUP_SIZE_X;
	remainder = numItemsInPrefixScanBuffer % PARALLEL_SORT_WORK_GROUP_SIZE_X;
	numWorkGroupsXByWorkGroupSize += (remainder == 0) ? 0 : 1;

	// working on a 1D array (X dimension), so these are always 1
	int numWorkGroupsY = 1;
	int numWorkGroupsZ = 1;

	// moving original data to intermediate data is 1 item per thread
	origDataToInterData.Enable();
	glDispatchCompute(numWorkGroupsXByWorkGroupSize, numWorkGroupsY, numWorkGroupsZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	origDataToInterData.Disable();

	// for 32bit unsigned integers, make 32 passes
	bool writeToSecondBuffer = true;
	for (unsigned int bitNumber = 0; bitNumber < 32; bitNumber++)
	{
		// this will either be 0 or half the size of IntermediateDataBuffer
		unsigned int intermediateDataReadBufferOffset = (unsigned int)!writeToSecondBuffer * numItemsInPrefixScanBuffer;
		unsigned int intermediateDataWriteBufferOffset = (unsigned int)writeToSecondBuffer * numItemsInPrefixScanBuffer;

		// getting 1 bit value from intermediate data to prefix sum is 1 item per thread
		bitForPrefixScan.Enable();
		glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_READ_OFFSET, intermediateDataReadBufferOffset);
		glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_WRITE_OFFSET, intermediateDataWriteBufferOffset);
		glUniform1ui(UNIFORM_LOCATION_BIT_NUMBER, bitNumber);
		glDispatchCompute(numWorkGroupsXByWorkGroupSize, numWorkGroupsY, numWorkGroupsZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		bitForPrefixScan.Disable();

		// prefix scan over all values
		// Note: Parallel prefix scan is 2 items per thread.
		parallelPrefixScan.Enable();
		glUniform1ui(UNIFORM_LOCATION_CALCULATE_ALL, 1);
		glDispatchCompute(numWorkGroupsXByItemsPerWorkGroup, numWorkGroupsY, numWorkGroupsZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// prefix scan over per-work-group sums
		// Note: The PrefixSumsByGroup array is sized to be exactly enough for 1 work group.  It 
		// makes the prefix sum easier than trying to eliminate excess threads.
		glUniform1ui(UNIFORM_LOCATION_CALCULATE_ALL, 0);
		glDispatchCompute(1, numWorkGroupsY, numWorkGroupsZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		parallelPrefixScan.Disable();

		// and sort the intermediate data with the scanned values
		sortInterData.Enable();
		glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_READ_OFFSET, intermediateDataReadBufferOffset);
		glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_WRITE_OFFSET, intermediateDataWriteBufferOffset);
		glUniform1ui(UNIFORM_LOCATION_BIT_NUMBER, bitNumber);
		glDispatchCompute(numWorkGroupsXByWorkGroupSize, numWorkGroupsY, numWorkGroupsZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		sortInterData.Disable();
		// now switch intermediate buffers and do it again
		writeToSecondBuffer = !writeToSecondBuffer;
	}

	// now use the sorted IntermediateData objects to sort the original data objects into a copy 
	// buffer (there is no "swap" in parallel sorting, so must write to a dedicated copy buffer
	sortOrigData.Enable();
	unsigned int intermediateDataReadBufferOffset = (unsigned int)!writeToSecondBuffer * numItemsInPrefixScanBuffer;
	glUniform1ui(UNIFORM_LOCATION_INTERMEDIATE_BUFFER_READ_OFFSET, intermediateDataReadBufferOffset);
	glDispatchCompute(numWorkGroupsXByWorkGroupSize, numWorkGroupsY, numWorkGroupsZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	sortOrigData.Disable();

	// and finally, move the sorted original data from the copy buffer back to the OriginalDataBuffer
	glBindBuffer(GL_COPY_READ_BUFFER, origDataCopyBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, particlesBuffer);
	unsigned int originalDataBufferSizeBytes = particlesCount * sizeof(SPHParticle);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, originalDataBufferSizeBytes);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void SPHGPUSolver::UpdateParams(SPHFluidParams &params, BoundaryBox &box)
{
	params.bucketSize = params.effectiveRadius * 2.f * 1.1f;
	params.bucketsCountX = static_cast<uint>(std::ceil((box.xMax - box.xMin) / (params.bucketSize)));
	params.bucketsCountY = static_cast<uint>(std::ceil((box.yMax - box.yMin) / (params.bucketSize)));

	origDataToInterData.Enable();
	glUniform1f(origDataToInterData.GetUniformLocation("cellSize"), params.bucketSize);
	glUniform1ui(origDataToInterData.GetUniformLocation("cellsCount"), params.bucketsCountX);
	origDataToInterData.Disable();

	accumulateForces.Enable();
	glUniform1f(accumulateForces.GetUniformLocation("h"), params.effectiveRadius);
	glUniform1f(accumulateForces.GetUniformLocation("mu"), params.viscocity);
	glUniform1f(accumulateForces.GetUniformLocation("s_tens_coeff"), -params.surfaceTension);
	glUniform1f(accumulateForces.GetUniformLocation("s_tens_tresh"), params.tensionTreshold);
	glUniform1f(accumulateForces.GetUniformLocation("g"), 9.81f);
	glUniform1ui(accumulateForces.GetUniformLocation("index_max_neighbors"), params.avgKernelParticles);
	accumulateForces.Disable();

	densityAndPressure.Enable();
	glUniform1f(densityAndPressure.GetUniformLocation("h"), params.effectiveRadius);
	glUniform1f(densityAndPressure.GetUniformLocation("k"), params.stiffness);
	glUniform1ui(densityAndPressure.GetUniformLocation("rd"), params.restDensity);
	glUniform1ui(densityAndPressure.GetUniformLocation("index_max_neighbors"), params.avgKernelParticles);
	densityAndPressure.Disable();

	indexUpdate.Enable();
	glUniform1ui(indexUpdate.GetUniformLocation("particlesCount"), params.particlesCount);
	indexUpdate.Disable();

	indexClear.Enable();
	glUniform1ui(indexClear.GetUniformLocation("particlesCount"), params.particlesCount);
	glUniform1ui(indexClear.GetUniformLocation("indexTableSize"), params.bucketsCountX * params.bucketsCountY);
	indexClear.Disable();

	leapfrogIntegration.Enable();
	glUniform1f(leapfrogIntegration.GetUniformLocation("dt"), params.dt);
	leapfrogIntegration.Disable();

	boundaries.Enable();
	glUniform1f(boundaries.GetUniformLocation("damping_coeff"), 0.7f);
	glUniform1f(boundaries.GetUniformLocation("boxMinX"), box.xMin);
	glUniform1f(boundaries.GetUniformLocation("boxMaxX"), box.xMax);
	glUniform1f(boundaries.GetUniformLocation("boxMinY"), box.yMin);
	glUniform1f(boundaries.GetUniformLocation("boxMaxY"), box.yMax);
	boundaries.Disable();

	render.Enable();
	glUniform1f(render.GetUniformLocation("k_ring_radius"), params.effectiveRadius);
	glUniform1f(render.GetUniformLocation("p_ring_radius"), params.particleRadius);
	glUniform1f(render.GetUniformLocation("k_ring_width"), 0.0005);
	glUniform1f(render.GetUniformLocation("p_ring_width"), 0.0005);
	render.Disable();
}

void SPHGPUSolver::Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	SortParticles();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, indexBuffer);

	indexClear.Enable();
	glDispatchCompute(params.bucketsCountX * params.bucketsCountY / WORK_GROUP_SIZE + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	indexClear.Disable();

	indexUpdate.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	indexUpdate.Disable();

	densityAndPressure.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	densityAndPressure.Disable();

	accumulateForces.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	accumulateForces.Disable();

	leapfrogIntegration.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	leapfrogIntegration.Disable();

	boundaries.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	boundaries.Disable();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);
}

void SPHGPUSolver::Render(glm::mat4 *projection, glm::mat4 * view)
{
	render.Enable();
	glUniformMatrix4fv(render.GetUniformLocation("mvp"), 1, GL_FALSE, &((*projection) * (*view))[0][0]);
	glUniformMatrix4fv(render.GetUniformLocation("p"), 1, GL_FALSE, &((*projection))[0][0]);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, particlesCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	render.Disable();
}

void SPHGPUSolver::UseBoundary(BoundaryBox &box, std::vector<SPHParticle> &p)
{
	
}