#pragma once
#include "glm/glm.hpp"


struct SPHParticle
{
	glm::dvec3 pos;
	glm::dvec3 vel;
	glm::dvec3 acc;
	double density;
};

