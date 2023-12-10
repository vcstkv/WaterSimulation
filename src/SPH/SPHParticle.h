#pragma once
#include "glm/glm.hpp"

struct SPHParticle {
    glm::dvec2 pos{};
    glm::dvec2 vel{};
    glm::dvec2 force{};
    glm::dvec2 prevForce{};
    double pressure{};
    double density{};
    double mass{};
    unsigned int bucket{};
    unsigned int pad;
};

struct SPHParticleSortIntermediateData {
    SPHParticleSortIntermediateData() : _data(0), _globalIndexOfOriginalData(0) {}

    unsigned int _data;
    unsigned int _globalIndexOfOriginalData;
};

struct SPHParticleIndex {
    explicit SPHParticleIndex(unsigned int particlesCount) : begin(particlesCount), end(0) {}

    unsigned int begin;
    unsigned int end;
    unsigned int n[9] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                         0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
};
