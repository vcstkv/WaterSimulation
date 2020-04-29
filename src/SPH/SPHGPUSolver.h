#pragma once
#include <vector>

#include "SPH/SPHParticle.h"
#include "SPH/SPHFluidParams.h"
#include "Graphics/Shaders/ShaderProgram.h"

class SPHGPUSolver
{
public:
	SPHGPUSolver(SPHFluidParams &params, BoundaryBox &box, std::vector<SPHParticle> &particles);
	~SPHGPUSolver();
	void UpdateParams(SPHFluidParams &params, BoundaryBox &box);
	void Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	void Render(const glm::mat4 &projection, const glm::mat4 &view);
	void UseBoundary(BoundaryBox &box, std::vector<SPHParticle> &p);
	void SortParticles();
private:
	ShaderProgram accumulateForces;
	ShaderProgram densityAndPressure;
	ShaderProgram indexClear;
	ShaderProgram indexUpdate;
	ShaderProgram leapfrogIntegration;
	ShaderProgram boundaries;
	ShaderProgram render;
	//-----------Compute shaders for gpu radix sort--------
	ShaderProgram origDataToInterData;
	ShaderProgram bitForPrefixScan;
	ShaderProgram parallelPrefixScan;
	ShaderProgram sortInterData;
	ShaderProgram sortOrigData;
	//-----------------------------------------------------
	const char *renderVertexShaderPath =	"sph//ParticleRender.vs";
	const char *renderGeometryShaderPath =	"sph//ParticleRender.gs";
	const char *renderFragmentShaderPath =	"sph//ParticleRender.fs";
	const char *accumulateForcesPath =		"sph//accumulate_forces.cs";
	const char *densityAndPressurePath =	"sph//density_and_pressure.cs";
	const char *indexClearPath =			"sph//index_clear.cs";
	const char *indexUpdatePath =			"sph//index_update.cs";
	const char *leapfrogIntegrationPath =	"sph//leapfrog_integration.cs";
	const char *boundariesPath =			"sph//reflect_boundaries.cs";

	uint particlesCount;
	GLuint particlesBuffer;
	GLuint indexBuffer;
	GLuint vao;
	GLuint prefixSumBuffer;
	GLuint origDataCopyBuffer;
	GLuint interDataBuffer;
	uint prefixSumBufferSize;
};

