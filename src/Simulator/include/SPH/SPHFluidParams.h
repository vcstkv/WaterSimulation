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
	unsigned int particlesCount;
	
};