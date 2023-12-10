#define _USE_MATH_DEFINES
#include "SPH/SPHSolver.h"

#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

void SPHSolver::Advect(SPHFluidParams& params, std::vector<SPHParticle>& particles) {
    UpdateDensity(params, particles);
    UpdatePosition(params, particles);
}

void SPHSolver::UpdateDensity(SPHFluidParams& params, std::vector<SPHParticle>& particles) {
    double distance;
    for (uint32_t i = 0; i < particles.size(); i++) {
        particles[i].density = params.restDensity;
        for (uint32_t j = 0; j < particles.size(); j++) {
            if (j == i) {
                continue;
            }
            distance = glm::distance2(particles[i].pos, particles[j].pos);
            particles[i].density +=
                params.particleMass * KernelPoly6(distance, params.effectiveRadius);
        }
        // particles[i].density = std::fmax(particles[i].density, params.restDensity);
    }
}

void SPHSolver::UpdatePosition(SPHFluidParams& params, std::vector<SPHParticle>& particles) {
    glm::dvec2 oldAcc;
    double magnitude;
    for (uint32_t i = 0; i < particles.size(); i++) {
        oldAcc = particles[i].force;
        particles[i].force =
            (InternalForces(params, particles, i) + ExternalForces(params, particles, i)) /
            particles[i].density;  //+ ExternalForces(params, particles, i) / params.particleMass;
        magnitude = glm::length(particles[i].force);
        if (magnitude > params.maxAcc) {
            particles[i].force *= params.maxAcc / magnitude;
        }
        particles[i].pos += (particles[i].vel + oldAcc / 2. * params.dt) * params.dt;
        particles[i].vel += (particles[i].force + oldAcc) / 2. * params.dt;
        magnitude = glm::length(particles[i].vel);
        if (magnitude > params.maxVel) {
            particles[i].vel *= params.maxVel / magnitude;
        }
    }
}

void SPHSolver::UseBoundary(BoundaryBox& box, std::vector<SPHParticle>& p) {
    for (uint32_t i = 0; i < p.size(); i++) {
        double eps = 0.000;
        double d = 0.6;
        if (p[i].pos.x < box.xMin) {
            p[i].pos = glm::dvec2(box.xMin + eps, p[i].pos.y);
            p[i].vel = glm::dvec2(-d * p[i].vel.x, p[i].vel.y);
        } else if (p[i].pos.x > box.xMax) {
            p[i].pos = glm::dvec2(box.xMax - eps, p[i].pos.y);
            p[i].vel = glm::dvec2(-d * p[i].vel.x, p[i].vel.y);
        }
        if (p[i].pos.y <= box.yMin) {
            p[i].pos = glm::dvec2(p[i].pos.x, box.yMin + eps);
            p[i].vel = glm::dvec2(p[i].vel.x, -d * p[i].vel.y);
        } else if (p[i].pos.y > box.yMax) {
            p[i].pos = glm::dvec2(p[i].pos.x, box.yMax - eps);
            p[i].vel = glm::dvec2(p[i].vel.x, -d * p[i].vel.y);
        }
    }
}

double SPHSolver::Pressure(SPHFluidParams& params, SPHParticle& p) {
    return /*params.stiffness * params.stiffness * params.restDensity / 7. *
              (pow(p.density/params.restDensity, 7.) - 1)*/
        params.stiffness * params.stiffness * (p.density - params.restDensity);
}

glm::dvec2 SPHSolver::InternalForces(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum) {
    return PressureForce(params, particles, particleNum) +
           ViscosityForce(params, particles, particleNum);
}

glm::dvec2 SPHSolver::ExternalForces(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum) {
    return GravityForce(params, particles, particleNum) +
           SurfaceTensionForce(params, particles, particleNum);
}

glm::dvec2 SPHSolver::PressureForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                    uint32_t particleNum) {
    glm::dvec2 force(0);
    for (uint32_t i = 0; i < particles.size(); i++) {
        if (i == particleNum) {
            continue;
        }
        force +=
            KernelPressure(particles[i].pos - particles[particleNum].pos, params.effectiveRadius) *
            (Pressure(params, particles[particleNum]) + Pressure(params, particles[i])) /
            (2.f * particles[i].density /** particles[particleNum].density*/) * params.particleMass;
    }

    return force;
}

glm::dvec2 SPHSolver::ViscosityForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                     uint32_t particleNum) {
    glm::dvec2 force(0);

    for (uint32_t i = 0; i < particles.size(); i++) {
        if (i == particleNum) {
            continue;
        }
        force += (particles[i].vel - particles[particleNum].vel) * params.particleMass /
                 particles[i].density *
                 KernelViscosity(glm::distance(particles[particleNum].pos, particles[i].pos),
                                 params.effectiveRadius);
    }

    return force * params.viscocity;
}

glm::dvec2 SPHSolver::SurfaceTensionForce(SPHFluidParams& params,
                                          std::vector<SPHParticle>& particles,
                                          uint32_t particleNum) {
    double c = 0;
    glm::dvec2 n(0);
    glm::dvec2 kernel(0);
    for (uint32_t i = 0; i < particles.size(); i++) {
        if (particleNum == i) {
            continue;
        }
        kernel =
            KernelGradPoly6(particles[particleNum].pos - particles[i].pos, params.effectiveRadius);
        n += kernel * params.particleMass / particles[i].density;
        c += glm::length(kernel) * params.particleMass / particles[i].density;
    }
    if (glm::length(n) > params.tensionTreshold) {
        return glm::normalize(n) * -params.surfaceTension * c;
    }

    return glm::dvec2(0);
}

glm::dvec2 SPHSolver::GravityForce(SPHFluidParams& params, std::vector<SPHParticle>& particles,
                                   uint32_t particleNum) {
    return glm::dvec2(0., -9.8) * particles[particleNum].density;
}

// Common kernel
double SPHSolver::KernelPoly6(double r2, double h) {
    if (r2 < 0 || r2 > h * h) {
        return 0.0;
    }
    return 4.921875 * M_1_PI / std::pow(h, 9) * std::pow((h * h - r2), 3);
}

glm::dvec2 SPHSolver::KernelGradPoly6(const glm::dvec2& r, double h) {
    double d = glm::length(r);
    if (d < 0 || d > h) {
        return glm::dvec2(0);
    }
    if (d <= 0.00001f) {
        d = 0.00001f;
    }
    return r * (-945.f / (32.f * M_PI * std::pow(h, 9) * d) * std::pow((h * h - d * d), 2));
}

double SPHSolver::KernelLaplPoly6(double r, double h) {
    if (r < 0 || r > h) {
        return 0.0;
    }
    return 945.f / (32.f * M_PI * std::pow(h, 9)) * (h * h - r * r) * (3 * h * h - 7 * r * r);
}

// Kernel for pressure
glm::dvec2 SPHSolver::KernelPressure(const glm::dvec2& r, double h) {
    double d = glm::length(r);
    if (d < 0 || d > h) {
        return glm::dvec2(0);
    }
    if (d <= 0.00001) {
        d = 0.00001;
    }
    return r * (-45.f / (M_PI * std::pow(h, 6) * d) * std::pow(h - d, 2));
}

// Kernel for viscosity
double SPHSolver::KernelViscosity(double r, double h) {
    if (r < 0 || r > h) {
        return 0.0;
    }
    return 45. / (M_PI * std::pow(h, 6)) * (h - r);
}
