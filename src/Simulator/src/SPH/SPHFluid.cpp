#include "SPH/SPHFluid.h"
#include "Graphics/Text/TextFont.h"

TextFont *f;
SPHFluid::SPHFluid(SPHFluidParams &params, BoundaryBox &box)
{
	this->params = params;
	particles = new std::vector<SPHParticle>(params.particlesCount);
	f = new TextFont("..\\data\\Fonts\\arial\\arial.fnt");
	f->SetParamValue(&glm::vec4(0.6, -6., 0.25, 34));
	solver = nullptr;
	this->boundaryBox = box;
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
		solver->UpdateParams(params, boundaryBox);
	}
}

void SPHFluid::Init()
{
	double ratio = 1. / 1.;
	uint h = static_cast<uint>(ceil(sqrt(1. * particles->size() / ratio)));
	uint w = static_cast<uint>(ratio * h);
	for (uint i = 0; i < h; i++)
	{
		for (uint j = 0; j < w; j++)
		{
			if (w * i + j >= particles->size())
			{
				break;
			}
			SPHParticle &p = particles->at(w * i + j);
			p.pos = 
				glm::vec2
				(
				    2.f * 2.2f * params.particleRadius * (j + 3.f / 2.f) + 0.1f/*+ 500*/,
					2.f * 2.2f * params.particleRadius * (i + 3.f / 2.f) + 0.1f/*+ 400*/
				);
			p.vel = glm::vec2(0.);
			p.force = glm::vec2(0.);
			p.prevForce = glm::vec2(0.);
			p.density = 0.f;
			p.pressure = 0.f;
			p.mass = params.particleMass;
		}
	}
	if (!solver)
	{
		solver = new SPHGPUSolver(params, boundaryBox, *particles);
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
