#pragma once
#include <vector>

#include "SPH/SPHFluidParams.h"
#include "SPH/SPHParticle.h"

class SPHSolver {
public:
    static void Advect(SPHFluidParams& params, std::vector<SPHParticle>& particles);
    static void UpdateDensity(SPHFluidParams& params, std::vector<SPHParticle>& particles);
    static void UpdateVelocity(SPHFluidParams& params, std::vector<SPHParticle>& particles);
    static void UpdatePosition(SPHFluidParams& params, std::vector<SPHParticle>& particles);
    static void UseBoundary(BoundaryBox& box, std::vector<SPHParticle>& p);

private:
    static double Pressure(SPHFluidParams& params, SPHParticle& p);
    static glm::dvec2 InternalForces(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum);
    static glm::dvec2 ExternalForces(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum);
    static glm::dvec2 PressureForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                    uint32_t particleNum);
    static glm::dvec2 ViscosityForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum);
    static glm::dvec2 SurfaceTensionForce(SPHFluidParams& params,
                                          std::vector<SPHParticle>& particles,
                                          uint32_t particleNum);
    static glm::dvec2 GravityForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                   uint32_t particleNum);

    static double KernelPoly6(double r2, double h);
    static glm::dvec2 KernelGradPoly6(const glm::dvec2& r, double h);
    static double KernelLaplPoly6(double r, double h);
    static glm::dvec2 KernelPressure(const glm::dvec2& r, double h);
    static double KernelViscosity(double r, double h);
};
