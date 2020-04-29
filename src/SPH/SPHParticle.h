#pragma once
#include "glm/glm.hpp"

struct SPHParticle
{
	SPHParticle() :
		pos(0.),
		vel(0.),
		force(0.),
		prevForce(0.),
		pressure(0.),
		density(0.),
		mass(0.),
		bucket(0)
	{

	}
	glm::vec2 pos;
	glm::vec2 vel;
	glm::vec2 force;
	glm::vec2 prevForce;
	float pressure;
	float density;
	float mass;
	unsigned int bucket;
};

struct SPHParticleSortIntermediateData
{
	SPHParticleSortIntermediateData() :
		_data(0),
		_globalIndexOfOriginalData(0)
	{
	}

	unsigned int _data;
	unsigned int _globalIndexOfOriginalData;
};

struct SPHParticleIndex
{
	SPHParticleIndex(unsigned int particlesCount) :
		begin(particlesCount),
		end(0)
	{
	}

	unsigned int begin;
	unsigned int end;
	unsigned int n[9] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
};

