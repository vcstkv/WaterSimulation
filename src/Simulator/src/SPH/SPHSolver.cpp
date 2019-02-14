#define _USE_MATH_DEFINES
#include "SPH/SPHSolver.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "cmath"
#include <algorithm>

typedef unsigned int uint;

void SPHSolver::Advect(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	UpdateDensity(params, particles);
	UpdatePosition(params, particles);
}

void SPHSolver::UpdateDensity(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	double distance;
	for (uint i = 0; i < particles.size(); i++)
	{
		particles[i].density = params.restDensity;
		for (uint j = 0; j < particles.size(); j++)
		{
			if (j == i)
			{
				continue;
			}
			distance = glm::distance2(particles[i].pos, particles[j].pos);
			particles[i].density += params.particleMass * KernelPoly6(distance, params.effectiveRadius);
		}
		//particles[i].density = std::fmax(particles[i].density, params.restDensity);
	}
}

void SPHSolver::UpdatePosition(SPHFluidParams &params, std::vector<SPHParticle> &particles)
{
	glm::dvec3 oldAcc;
	double magnitude;
	for (uint i = 0; i < particles.size(); i++)
	{
		oldAcc = particles[i].acc;
		particles[i].acc = (InternalForces(params, particles, i) + ExternalForces(params, particles, i)) / particles[i].density; //+ ExternalForces(params, particles, i) / params.particleMass;
		magnitude = glm::length(particles[i].acc);
		if (magnitude > params.maxAcc)
		{
			particles[i].acc *= params.maxAcc / magnitude;
		}
		particles[i].pos += (particles[i].vel + oldAcc / 2. * params.dt) * params.dt;
		particles[i].vel += (particles[i].acc + oldAcc) / 2. * params.dt;	
		magnitude = glm::length(particles[i].vel);
		if (magnitude > params.maxVel)
		{
			particles[i].vel *= params.maxVel / magnitude;
		}
	}
}

void SPHSolver::UseBoundary(BoundaryBox &box, std::vector<SPHParticle> &p)
{
	for (uint i = 0; i < p.size(); i++)
	{
		double eps = 0.000;
		float d = 0.6;
		if (p[i].pos.x < box.xMin) 
		{
			p[i].pos = glm::dvec3(box.xMin + eps, p[i].pos.y, p[i].pos.z);
			p[i].vel = glm::dvec3(-d * p[i].vel.x, p[i].vel.y, p[i].vel.z);
		}
		else if (p[i].pos.x > box.xMax) 
		{
			p[i].pos = glm::dvec3(box.xMax - eps, p[i].pos.y, p[i].pos.z);
			p[i].vel = glm::dvec3(-d * p[i].vel.x, p[i].vel.y, p[i].vel.z);
		}
		if (p[i].pos.y <= box.yMin) 
		{
			p[i].pos = glm::dvec3(p[i].pos.x, box.yMin + eps, p[i].pos.z);
			p[i].vel = glm::dvec3(p[i].vel.x, -d * p[i].vel.y, p[i].vel.z);
		}
		else if (p[i].pos.y > box.yMax) 
		{
			p[i].pos = glm::dvec3(p[i].pos.x, box.yMax - eps, p[i].pos.z);
			p[i].vel = glm::dvec3(p[i].vel.x, -d * p[i].vel.y, p[i].vel.z);
		}
		if (p[i].pos.z < box.zMin)
		{
			p[i].pos = glm::dvec3(p[i].pos.x, p[i].pos.y, box.zMin + eps);
			p[i].vel = glm::dvec3(p[i].vel.x, p[i].vel.y, -d * p[i].vel.z);
		}
		else if (p[i].pos.z > box.zMax)
		{
			p[i].pos = glm::dvec3(p[i].pos.x, p[i].pos.y, box.zMax - eps);
			p[i].vel = glm::dvec3(p[i].vel.x, p[i].vel.y, -d * p[i].vel.z);
		}
	}
}

double SPHSolver::Pressure(SPHFluidParams &params, SPHParticle &p)
{
	return /*params.stiffness * params.stiffness * params.restDensity / 7. * (pow(p.density/params.restDensity, 7.) - 1)*/
		params.stiffness * params.stiffness * (p.density - params.restDensity);
}

glm::dvec3 SPHSolver::InternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	return PressureForce(params, particles, particleNum) 
		 + ViscosityForce(params, particles, particleNum);
}

glm::dvec3 SPHSolver::ExternalForces(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	return GravityForce(params, particles, particleNum) 
		 + SurfaceTensionForce(params, particles, particleNum);
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
		force += KernelPressure(particles[i].pos - particles[particleNum].pos, params.effectiveRadius)
			* (Pressure(params, particles[particleNum]) + Pressure(params, particles[i])) / (2. * particles[i].density /** particles[particleNum].density*/)
			* params.particleMass;
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

	return force * params.viscocity;
}

glm::dvec3 SPHSolver::SurfaceTensionForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
{
	double c = 0;
	glm::dvec3 n(0);
	glm::dvec3 kernel(0);
	for (uint i = 0; i < particles.size(); i++)
	{
		if (particleNum == i)
		{
			continue;
		}
		kernel = KernelGradPoly6(particles[particleNum].pos - particles[i].pos, params.effectiveRadius);
		n += kernel * params.particleMass / particles[i].density;
		c += glm::length(kernel) * params.particleMass / particles[i].density;
	}
	if (glm::length(n) > params.tensionTreshold)
	{
		return glm::normalize(n) * -params.surfaceTension * c;
	}
	
	return glm::dvec3(0);
}

glm::dvec3 SPHSolver::GravityForce(SPHFluidParams &params, std::vector<SPHParticle> &particles, uint particleNum)
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

glm::dvec3 SPHSolver::KernelGradPoly6(glm::dvec3 &r, double h)
{
	double d = glm::length(r);
	if (d < 0 || d > h)
	{
		return glm::dvec3(0);
	}
	if (d <= 0.00001)
	{
		d = 0.00001;
	}
	return r * (-945. / (32. * M_PI * pow(h, 9) * d) * pow((h * h - d * d), 2));
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
glm::dvec3 SPHSolver::KernelPressure(glm::dvec3 &r, double h)
{
	double d = glm::length(r);
	if (d < 0 || d > h)
	{
		return glm::dvec3(0);
	}
	if (d <= 0.00001)
	{
		d = 0.00001;
	}
	return r * (-45. / (M_PI * pow(h, 6) * d) * pow(h - d, 2));
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
