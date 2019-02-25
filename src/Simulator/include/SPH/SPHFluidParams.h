#pragma once

typedef unsigned int uint;

struct SPHFluidParams
{
	float viscocity;
	float particleMass;
	float restPressure;
	float restDensity;
	float effectiveRadius;
	float surfaceTension;
	float tensionTreshold;
	float stiffness;
	float dt;
	float particleRadius;
	float distanceThreshold;
	float maxAcc;
	float maxVel;
	uint avgKernelParticles;
	unsigned int particlesCount;
};

typedef struct
{
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double zMin;
	double zMax;
} BoundaryBox;