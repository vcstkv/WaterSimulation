#include "SPH/SPHFluid.h"
#include "Graphics/Text/TextFont.h"


TextFont *f;
SPHFluid::SPHFluid(SPHFluidParams &params)
{
	this->params = params;
	particles = new std::vector<SPHParticle>(params.particlesCount);
	f = new TextFont("..\\data\\Fonts\\arial\\arial.fnt");
	f->SetParamValue(&glm::vec4(0.6, -6., 0.25, 34));
	Init();
}

SPHFluid::~SPHFluid()
{
	delete particles;
}

void SPHFluid::Update(double delta)
{
	SPHSolver::Advect(params, *particles);
	SPHSolver::UseBoundary(boundaryBox, *particles);
}

void SPHFluid::Render(Graphics *graphics, glm::mat4 *projection, glm::mat4 *view)
{
	for (uint i = 0; i < particles->size(); i++)
	{
		graphics->DrawCircle(particles->at(i).pos.x,
							 particles->at(i).pos.y,
							 params.particleRadius,
							 0.0008,
							 &glm::vec4(0, 0, 1, 1),
							 projection,
							 view);
	}

	/*for (uint i = 0; i < particles->size(); i++)
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
							 0.0008,
							 &glm::vec4(0, 1, 0, 1),
							 projection,
							 view);
	}
	f->DrawText(L"density [0]: " + std::to_wstring(particles->at(0).density), 0.035, &glm::vec4(0, 0, 0, 1), 0.03, 0.3, projection);*/
}

void SPHFluid::DragParticle(double x, double y, uint particleNum)
{
	if (particleNum >= particles->size())
	{
		return;
	}
	particles->at(particleNum).pos.x = x;
	particles->at(particleNum).pos.y = y;
	particles->at(particleNum).vel.x = 0;
	particles->at(particleNum).vel.y = 0;
}

void SPHFluid::SetBoundaryBox(BoundaryBox & box)
{
	boundaryBox = box;
}

void SPHFluid::Init()
{
	uint initGridSize = static_cast<uint>(ceil(sqrt(particles->size())));
	for (uint i = 0; i < initGridSize; i++)
	{
		for (uint j = 0; j < initGridSize; j++)
		{
			if (initGridSize * i + j >= particles->size())
			{
				return;
			}
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

void SPHFluid::AddParticle(double x, double y, double z)
{
	SPHParticle p;
	p.pos.x = x;
	p.pos.y = y;
	p.pos.z = z;
	p.vel = glm::dvec3(0.);
	p.acc = glm::dvec3(0.);
	p.density = 0.;
	particles->push_back(p);
}
