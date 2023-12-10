#pragma once

#include <cstdint>

struct SPHFluidParams {
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
    double bucketSize;
    int32_t bucketsCountX;
    int32_t bucketsCountY;
    int32_t avgKernelParticles;
    int32_t particlesCount;
};

typedef struct {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double zMin;
    double zMax;
} BoundaryBox;