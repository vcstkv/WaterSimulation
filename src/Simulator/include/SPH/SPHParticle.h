#pragma once
#include "glm/glm.hpp"


struct SPHParticle
{
	glm::vec2 pos;
	glm::vec2 vel;
	glm::vec2 force;
	glm::vec2 prevForce;
	float pressure;
	float density;
	float mass;
	float _;
};

