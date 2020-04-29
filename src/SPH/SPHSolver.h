#pragma once
#include <vector>

#include "SPH/SPHParticle.h"
#include "SPH/SPHFluidParams.h"

class SPHSolver
{
public:
	static void Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	static void UpdateDensity(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	static void UpdateVelocity(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	static void UpdatePosition(SPHFluidParams &params, std::vector<SPHParticle> &particles);
	static void UseBoundary(BoundaryBox &box, std::vector<SPHParticle> &p);
private:
	static float Pressure(SPHFluidParams &params, SPHParticle &p);
	static glm::vec2 InternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::vec2 ExternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::vec2 PressureForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::vec2 ViscosityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::vec2 SurfaceTensionForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::vec2 GravityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);

	static float KernelPoly6(float r2, float h);
	static glm::vec2 KernelGradPoly6(const glm::vec2 &r, float h);
	static float KernelLaplPoly6(float r, float h);
	static glm::vec2 KernelPressure(const glm::vec2 &r, float h);
	static float KernelViscosity(float r, float h);
};

