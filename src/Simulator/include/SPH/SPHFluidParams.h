#pragma once



typedef unsigned int uint;

struct SPHFluidParams
{
	double viscocity;
	double particleMass;
	double restPressure;
	double restDensity;
	double effectiveRadius;
	double surfaceTension;
	double tensionTreshold;
	double stiffness;
	double dt;
	double particleRadius;
	double distanceThreshold;
	double maxAcc;
	double maxVel;
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