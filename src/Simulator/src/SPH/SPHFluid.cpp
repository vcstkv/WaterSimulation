#include "SPH/SPHFluid.h"

SPHFluid::SPHFluid(SPHFluidParams &params)
{
	this->params = params;
	particles = new std::vector<SPHParticle>(params.particlesCount);
	Init();
}

SPHFluid::~SPHFluid()
{
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
		graphics->DrawCircle(particles->at(i).pos.x,
							 particles->at(i).pos.y,
							 params.particleRadius,
							 params.particleRadius * 0.05,
							 &glm::vec4(0, 0, 1, 1),
							 projection,
							 view);
	}

	for (uint i = 0; i < particles->size(); i++)
	{
		graphics->DrawLine(particles->at(i).pos.x,
			particles->at(i).pos.y,
			particles->at(i).pos.x + particles->at(i).acc.x,
			particles->at(i).pos.y + particles->at(i).acc.y,
			&glm::vec4(1, 0, 0, 1),
			projection);

		graphics->DrawCircle(particles->at(i).pos.x,
							 particles->at(i).pos.y,
							 params.effectiveRadius,
							 params.effectiveRadius * 0.05,
							 &glm::vec4(0, 1, 0, 1),
							 projection,
							 view);
	}
}

void SPHFluid::DragParticle(double x, double y, uint particleNum)
{
	if (particleNum >= particles->size())
	{
		return;
	}
	particles->at(particleNum).pos.x = x;
	particles->at(particleNum).pos.y = y;
}

void SPHFluid::Init()
{
	uint initGridSize = static_cast<uint>(sqrt(particles->size()));
	for (uint i = 0; i < initGridSize; i++)
	{
		for (uint j = 0; j < initGridSize; j++)
		{
			particles->at(initGridSize * i + j).pos = 
				glm::dvec3
				(
				    2. * params.particleRadius * (j + 3. / 2.) + 0.2/*+ 500*/,
					2. * params.particleRadius * (i + 3. / 2.) + 0.1/*+ 400*/,
					0.
				);
			particles->at(initGridSize * i + j).vel = glm::dvec3(0.);
			particles->at(initGridSize * i + j).acc = glm::dvec3(0.);
			particles->at(initGridSize * i + j).density = 0.;
		}
	}
}
