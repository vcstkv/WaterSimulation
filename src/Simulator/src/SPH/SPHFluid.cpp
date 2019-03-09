#include "SPH/SPHFluid.h"
#include "Graphics/Text/TextFont.h"

TextFont *f;
SPHFluid::SPHFluid(SPHFluidParams &params)
{
	this->params = params;
	particles = new std::vector<SPHParticle>(params.particlesCount);
	f = new TextFont("..\\data\\Fonts\\arial\\arial.fnt");
	f->SetParamValue(&glm::vec4(0.6, -6., 0.25, 34));
	solver = nullptr;
	Init();
}

SPHFluid::~SPHFluid()
{
	delete particles;
}

void SPHFluid::Update(float delta)
{
	solver->Advect(params, *particles);
	//solver->UseBoundary(boundaryBox, *particles);
}

void SPHFluid::Render(Graphics *graphics, glm::mat4 *projection, glm::mat4 *view)
{
	solver->Render(projection, view);
	//for (uint i = 0; i < particles->size(); i++)
	//{
	//	graphics->DrawCircle(particles->at(i).pos.x,
	//						 particles->at(i).pos.y,
	//						 /*0.8 * params.effectiveRadius*/params.particleRadius,
	//						 /*0.8 * params.effectiveRadius*/0.0008,
	//						 &glm::vec4(0, 0, 1, 1),
	//						 projection,
	//						 view);
	//}

	//for (uint i = 0; i < particles->size(); i++)
	//{
	//	/*graphics->DrawLine(particles->at(i).pos.x,
	//		particles->at(i).pos.y,
	//		particles->at(i).pos.x + particles->at(i).acc.x,
	//		particles->at(i).pos.y + particles->at(i).acc.y,
	//		&glm::vec4(1, 0, 0, 1),
	//		projection);*/

	//	graphics->DrawCircle(particles->at(i).pos.x,
	//						 particles->at(i).pos.y,
	//						 params.effectiveRadius,
	//						 0.0008,
	//						 &glm::vec4(0, 1, 0, 1),
	//						 projection,
	//						 view);
	//}
	//f->DrawText(L"density [0]: " + std::to_wstring(particles->at(0).density), 24, &glm::vec4(0, 0, 0, 1), 100, 500, projection);
}

void SPHFluid::DragParticle(float x, float y, uint particleNum)
{
	particles->at(particleNum).pos.x = x;
	particles->at(particleNum).pos.y = y;
	particles->at(particleNum).vel.x = 0;
	particles->at(particleNum).vel.y = 0;
}

void SPHFluid::SetBoundaryBox(BoundaryBox & box)
{
	boundaryBox = box;
}

void SPHFluid::AdjustParams(SPHFluidParams &params)
{
	this->params = params;
	if (solver)
	{
		solver->UpdateParams(params);
	}
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
				break;
			}
			particles->at(initGridSize * i + j).pos = 
				glm::vec2
				(
				    2.f * params.particleRadius * (j + 3.f / 2.f) + 0.2f/*+ 500*/,
					2.f * params.particleRadius * (i + 3.f / 2.f) + 0.1f/*+ 400*/
				);
			particles->at(initGridSize * i + j).vel = glm::vec2(0.);
			particles->at(initGridSize * i + j).force = glm::vec2(0.);
			particles->at(initGridSize * i + j).prevForce = glm::vec2(0.);
			particles->at(initGridSize * i + j).density = 0.f;
			particles->at(initGridSize * i + j).pressure = 0.f;
			particles->at(initGridSize * i + j).mass = params.particleMass;
		}
	}
	if (!solver)
	{
		solver = new SPHGPUSolver(params, *particles);
	}
}

void SPHFluid::AddParticle(float x, float y, float z)
{
	SPHParticle p;
	p.pos.x = x;
	p.pos.y = y;
	p.vel = glm::vec2(0.8, -0.8);
	p.force = glm::vec2(0.);
	p.prevForce = glm::vec2(0.);
	p.density = 0.f;
	particles->push_back(p);
}
