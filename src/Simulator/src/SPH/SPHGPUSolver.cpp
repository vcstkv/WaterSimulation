#define _USE_MATH_DEFINES
#include "SPH/SPHGPUSolver.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "cmath"
#include <algorithm>

typedef unsigned int uint;

#define WORK_GROUP_SIZE 16

SPHGPUSolver::SPHGPUSolver(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	particlesCount = particles.size();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW init error");
	}

	glEnable(GL_PROGRAM_POINT_SIZE);

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

	UpdateParams(params);

	glGenBuffers(1, &particlesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SPHParticle) * particles.size(), particles.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint) * particles.size() * params.avgKernelParticles, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, particlesBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SPHParticle), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SPHGPUSolver::~SPHGPUSolver()
{
	glDeleteBuffers(1, &particlesBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void SPHGPUSolver::UpdateParams(SPHFluidParams & params)
{
	accumulateForces.Enable();
	glUniform1f(accumulateForces.GetUniformLocation("h"), params.effectiveRadius);
	glUniform1f(accumulateForces.GetUniformLocation("mu"), params.viscocity);
	glUniform1f(accumulateForces.GetUniformLocation("g"), 9.81f);
	glUniform1ui(accumulateForces.GetUniformLocation("index_max_neighbors"), params.avgKernelParticles);
	accumulateForces.Disable();

	densityAndPressure.Enable();
	glUniform1f(densityAndPressure.GetUniformLocation("h"), params.effectiveRadius);
	glUniform1f(densityAndPressure.GetUniformLocation("k"), params.stiffness);
	glUniform1ui(densityAndPressure.GetUniformLocation("index_max_neighbors"), params.avgKernelParticles);
	densityAndPressure.Disable();

	indexUpdate.Enable();
	glUniform1f(indexUpdate.GetUniformLocation("h"), params.effectiveRadius);
	glUniform1ui(indexUpdate.GetUniformLocation("index_max_neighbors"), params.avgKernelParticles);
	indexUpdate.Disable();

	leapfrogIntegration.Enable();
	glUniform1f(leapfrogIntegration.GetUniformLocation("dt"), params.dt);
	leapfrogIntegration.Disable();

	boundaries.Enable();
	glUniform1f(boundaries.GetUniformLocation("damping_coeff"), 0.9f);
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particlesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indexBuffer);

	indexClear.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	indexClear.Disable();

	indexUpdate.Enable();
	glDispatchCompute(particlesCount / WORK_GROUP_SIZE, particlesCount / WORK_GROUP_SIZE, 1);
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
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