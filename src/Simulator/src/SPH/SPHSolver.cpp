#define _USE_MATH_DEFINES
#include "SPH/SPHSolver.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "cmath"

typedef unsigned int uint;

void SPHSolver::Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	UpdateDensity(params, particles);
	UpdatePosition(params, particles);
}

void SPHSolver::UpdateDensity(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	for (uint i = 0; i < particles.size(); i++)
	{
		particles[i].density = 0;
		for (uint j = 0; j < particles.size(); j++)
		{
			/*if (j == i)
			{
				continue;
			}*/
			particles[i].density += params.particleMass * KernelPoly6(glm::distance2(particles[i].pos, particles[j].pos), params.effectiveRadius);

		}
	}
}

void SPHSolver::UpdatePosition(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	glm::dvec3 oldAcc;
	for (uint i = 0; i < particles.size(); i++)
	{
		oldAcc = particles[i].acc;
		particles[i].acc = (InternalForces(params, particles, i) /*+ ExternalForces(params, particles, i)*/) / particles[i].density;
		particles[i].pos += (particles[i].vel + oldAcc / 2. * params.dt) * params.dt;
		particles[i].vel += (particles[i].acc + oldAcc) / 2. * params.dt;	
	}
}

double SPHSolver::Pressure(SPHFluidParams &params, SPHParticle &p)
{
	return /*params.stiffness * params.stiffness * params.restDensity / 1. * (pow(p.density/params.restDensity, 1.) - 1)*/
		params.stiffness * params.stiffness * (p.density - params.restDensity);
}

glm::dvec3 SPHSolver::InternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	return PressureForce(params, particles, particleNum) 
		 /*+ ViscosityForce(params, particles, particleNum)*/;
}

glm::dvec3 SPHSolver::ExternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	return /*GravityForce(particles, particleNum) 
		 + */SurfaceTensionForce(params, particles, particleNum);
}

glm::dvec3 SPHSolver::PressureForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	glm::dvec3 force(0);

	for (uint i = 0; i < particles.size(); i++)
	{
		if (i == particleNum)
		{
			continue;
		}
		double pressure = (Pressure(params, particles[particleNum])
						 + Pressure(params, particles[i]))
					     / (2. * particles[i].density);
		double mass = params.particleMass;
		double kernel = KernelPressure(glm::distance(particles[particleNum].pos, particles[i].pos), params.effectiveRadius);
		force += glm::normalize(particles[i].pos - particles[particleNum].pos)
			* pressure
			* mass
			* kernel;
	}

	return force;
}

glm::dvec3 SPHSolver::ViscosityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	glm::dvec3 force(0);

	for (uint i = 0; i < particles.size(); i++)
	{
		if (i == particleNum)
		{
			continue;
		}
		force += (particles[i].vel - particles[particleNum].vel)
			* params.particleMass / particles[i].density
			* KernelViscosity(glm::distance(particles[particleNum].pos, particles[i].pos), params.effectiveRadius);
	}

	return force * params.viscocity / particles[particleNum].density;
}

glm::dvec3 SPHSolver::SurfaceTensionForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	double c = 0;
	glm::dvec3 n(0);
	for (uint i = 0; i < particles.size(); i++)
	{
		if (particleNum == i)
		{
			continue;
		}
		n += glm::normalize(particles[particleNum].pos - particles[i].pos)
			* params.particleMass / particles[i].density
			* KernelGradPoly6(glm::distance(particles[particleNum].pos, particles[i].pos), params.effectiveRadius);
		c += params.particleMass / particles[i].density
			* KernelLaplPoly6(glm::distance(particles[particleNum].pos, particles[i].pos), params.effectiveRadius);
	}
	if (glm::length(n) > params.tensionTreshold)
	{
		return glm::normalize(n) * -params.surfaceTension * c;
	}
	
	return glm::dvec3(0);
}

glm::dvec3 SPHSolver::GravityForce(std::vector<SPHParticle> &particles, uint particleNum)
{
	return glm::dvec3(0., -9.8, 0.) * particles[particleNum].density;
}

//Common kernel
double SPHSolver::KernelPoly6(double r2, double h)
{
	if (r2 < 0 || r2 > h * h)
	{
		return 0.0;
	}
	return 4.921875 * M_1_PI / pow(h, 9) * pow((h * h - r2), 3);
}

double SPHSolver::KernelGradPoly6(double r, double h)
{
	if (r < 0 || r > h)
	{
		return 0.0;
	}
	return -945. / (32. * M_PI * pow(h, 9)) * pow((h * h - r * r), 2);
}

double SPHSolver::KernelLaplPoly6(double r, double h)
{
	if (r < 0 || r > h)
	{
		return 0.0;
	}
	return 945. / (32. * M_PI * pow(h, 9)) * (h * h - r * r) * (3 * h * h - 7 * r * r);
}

//Kernel for pressure
double SPHSolver::KernelPressure(double r, double h)
{
	if (r < 0 || r > h)
	{
		return 0.0;
	}
	return -45. / (M_PI * pow(h, 6)) * pow(h - r, 2);
}

//Kernel for viscosity
double SPHSolver::KernelViscosity(double r, double h)
{
	if (r < 0 || r > h)
	{
		return 0.0;
	}
	return 45. / (M_PI * pow(h, 6)) * (h - r);
}
