#include "SPH/SPHFluid.h"

SPHFluid::SPHFluid(SPHFluidParams &params)
{
	this->params = params;
	sprite = new CommonSprite(0, 0);
	sprite->SetTexture("WaterParticle.png", true);
	sprite->SetShader("CommonSprite.vs", "CommonSprite.fs");
	sprite->Scale(&glm::vec3(2. * params.particleRadius/sprite->width));
	particles = new std::vector<SPHParticle>(params.particlesCount);
	Init();
}

SPHFluid::~SPHFluid()
{
	delete sprite;
	delete particles;
}

void SPHFluid::Update(double delta)
{
	SPHSolver::Advect(params, *particles);
}

void SPHFluid::Render(Graphics *graphics, glm::mat4 *projection, glm::mat4 *view)
{
	for (uint i = 0; i < particles->size(); i++)
	{
		sprite->SetPosition(particles->at(i).pos.x, particles->at(i).pos.y);
		sprite->Draw(projection, view);
	}
}

void SPHFluid::Init()
{
	uint initGridSize = static_cast<uint>(sqrtf(particles->size()));
	for (uint i = 0; i < initGridSize; i++)
	{
		for (uint j = 0; j < initGridSize; j++)
		{
			particles->at(initGridSize * i + j).pos = 
				glm::dvec3
				(
				    2.5 * params.particleRadius * (j + 3. / 2.) + 0.5/*+ 500*/,
					2.5 * params.particleRadius * (i + 3. / 2.) + 0.15/*+ 400*/,
					0.
				);
			particles->at(initGridSize * i + j).vel = glm::dvec3(0.);
			particles->at(initGridSize * i + j).acc = glm::dvec3(0.);
			particles->at(initGridSize * i + j).density = 0.;
		}
	}
}
