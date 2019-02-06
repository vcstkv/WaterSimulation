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
	static double Pressure(SPHFluidParams &params, SPHParticle &p);
	static glm::dvec3 InternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::dvec3 ExternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::dvec3 PressureForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::dvec3 ViscosityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::dvec3 SurfaceTensionForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);
	static glm::dvec3 GravityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum);

	static double KernelPoly6(double r2, double h);
	static glm::dvec3 KernelGradPoly6(glm::dvec3 &r, double h);
	static double KernelLaplPoly6(double r, double h);
	static glm::dvec3 KernelPressure(glm::dvec3 &r, double h);
	static double KernelViscosity(double r, double h);
};

