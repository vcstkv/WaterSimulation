#pragma once
#include <vector>

#include "SPH/SPHParticle.h"
#include "SPH/SPHFluidParams.h"
#include "Graphics/Shaders/ShaderProgram.h"
#include "Graphics/GLHeaders.h"

class SPHGPUSolver
{
public:
	SPHGPUSolver(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	~SPHGPUSolver();
	void UpdateParams(SPHFluidParams &params);
	void Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	void Render(glm::mat4 *projection, glm::mat4 *view);
	void UseBoundary(BoundaryBox &box, std::vector<SPHParticle> &p);
private:
	ShaderProgram accumulateForces;
	ShaderProgram densityAndPressure;
	ShaderProgram indexClear;
	ShaderProgram indexUpdate;
	ShaderProgram leapfrogIntegration;
	ShaderProgram boundaries;
	ShaderProgram render;
	const char *renderVertexShaderPath = "sph_old//ParticleRender.vs";
	const char *renderGeometryShaderPath = "sph_old//ParticleRender.gs";
	const char *renderFragmentShaderPath = "sph_old//ParticleRender.fs";
	const char *accumulateForcesPath = "sph_old//accumulate_forces.cs";
	const char *densityAndPressurePath = "sph_old//density_and_pressure.cs";
	const char *indexClearPath = "sph_old//index_clear.cs";
	const char *indexUpdatePath = "sph_old//index_update.cs";
	const char *leapfrogIntegrationPath = "sph_old//leapfrog_integration.cs";
	const char *boundariesPath = "sph_old//reflect_boundaries.cs";

	uint particlesCount;
	GLuint particlesBuffer;
	GLuint indexBuffer;
	GLuint vao;
};

