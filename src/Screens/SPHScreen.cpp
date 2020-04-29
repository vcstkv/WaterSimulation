#include "Screens/SPHScreen.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Graphics/Text/TextFont.h"
#include "GUI/Controlls/Slider.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

bool emitParticles = false;
glm::dvec2 cursorPos(0);
glm::mat4 guiview;
TextFont *fnt;
SPHFluidParams p;

Slider *restDensitySlider;
Slider *viscositySlider;
Slider *stiffnessSlider;
Slider *surfaceTensionSlider;
Slider *massSlider;
Slider *maxVelSlider;
Slider *maxAccSlider;
Slider *avgKernelParticlesSlider;

bool isAutoPlay = false;

struct ParamSliderData
{
	SPHFluidParams *p;
	SPHFluid *f;
};

ParamSliderData sd;

SPHScreen::~SPHScreen()
{
	delete restDensitySlider;
	delete viscositySlider;
	delete stiffnessSlider;
	delete surfaceTensionSlider;
	delete massSlider;
	delete maxVelSlider;
	delete maxAccSlider;
}

SPHScreen::SPHScreen(int width, int height) : Screen(width, height)
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	scaleCoeff = width / 6.0f;
	projection = glm::ortho(0.0f, width * 1.0f, 0.0f, height * 1.0f, 1.0f, -1.0f);
	view = /*glm::mat4(1);*/glm::scale(glm::mat4(1.f), glm::vec3(scaleCoeff, scaleCoeff, 1.f));
	guiview = glm::mat4(1.f);
}

int fps = 0;

void SPHScreen::Update(float delta)
{
	/*if (emitParticles)
	{
		fluid->AddParticle(cursorPos.x, cursorPos.y, 0.);
	}*/
	fps = static_cast<int>(round(1.f/delta));
	if (isAutoPlay)
	{
		fluid->Update(delta);
	}
}

bool isGUI = true;

void SPHScreen::Render(const std::shared_ptr<const Graphics> graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fluid->Render(graphics, projection, view);
	if (!isGUI)
	{
		return;
	}
	fnt->DrawText(L"density: " + std::to_wstring(restDensitySlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 50 + 24 + 10, projection);
	restDensitySlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"viscosity: " + std::to_wstring(viscositySlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 2 * 50 + 24 + 10, projection);
	viscositySlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"stiffness: " + std::to_wstring(stiffnessSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 3 * 50 + 24 + 10, projection);
	stiffnessSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"sTension: " + std::to_wstring(surfaceTensionSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 4 * 50 + 24 + 10, projection);
	surfaceTensionSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"mass: " + std::to_wstring(massSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 5 * 50 + 24 + 10, projection);
	massSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"maxVel: " + std::to_wstring(maxVelSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 6 * 50 + 24 + 10, projection);
	maxVelSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"maxAcc: " + std::to_wstring(maxAccSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 7 * 50 + 24 + 10, projection);
	maxAccSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"avgKerParticles: " + std::to_wstring(avgKernelParticlesSlider->GetValue()), 24, glm::vec4(0, 0, 0, 1), 0, height - 8 * 50 + 24 + 10, projection);
	avgKernelParticlesSlider->Draw(graphics, projection, guiview);
	fnt->DrawText(L"FPS: " + std::to_wstring(fps), 24, glm::vec4(0, 0, 0, 1), width - 100, height - 30, projection);
	//printf("err: %d", glGetError());
}

void SPHScreen::OnMouseButtonEvent(int btn, int action, int mods)
{
	restDensitySlider->OnMouseButtonEvent(btn, action, mods);
	viscositySlider->OnMouseButtonEvent(btn, action, mods);
	stiffnessSlider->OnMouseButtonEvent(btn, action, mods);
	surfaceTensionSlider->OnMouseButtonEvent(btn, action, mods);
	massSlider->OnMouseButtonEvent(btn, action, mods);
	maxVelSlider->OnMouseButtonEvent(btn, action, mods);
	maxAccSlider->OnMouseButtonEvent(btn, action, mods);
	avgKernelParticlesSlider->OnMouseButtonEvent(btn, action, mods);

	switch (btn)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		//dragParticle = action == GLFW_PRESS;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		emitParticles = action == GLFW_PRESS;
		/*if (action == GLFW_PRESS)
		{
			fluid->AddParticle(cursorPos.x, cursorPos.y, 0.);
		}*/
		break;
	}
}

void SPHScreen::OnMouseCursorEvent(double x, double y)
{
	restDensitySlider->OnMouseCursorEvent(x, height - y);
	viscositySlider->OnMouseCursorEvent(x, height - y);
	stiffnessSlider->OnMouseCursorEvent(x, height - y);
	surfaceTensionSlider->OnMouseCursorEvent(x, height - y);
	massSlider->OnMouseCursorEvent(x, height - y);
	maxVelSlider->OnMouseCursorEvent(x, height - y);
	maxAccSlider->OnMouseCursorEvent(x, height - y);
	avgKernelParticlesSlider->OnMouseCursorEvent(x, height - y);

	cursorPos.x = x / scaleCoeff;
	cursorPos.y = (height - y) / scaleCoeff;

	if (dragParticle)
	{
		fluid->DragParticle(cursorPos.x, cursorPos.y, 0);
	}
}

void SPHScreen::OnMouseScrollEvent(double offsetX, double offsetY)
{

}

void SPHScreen::OnKeyboardEvent(int key, int scanCode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ENTER:
		if (action == GLFW_PRESS)
		{
			//fluid->Init();
			isAutoPlay = !isAutoPlay;
		}
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			fluid->Update(0.);
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_REPEAT)
		{
			fluid->Update(0.);
		}
		break;
	case GLFW_KEY_H:
		if (action == GLFW_PRESS)
		{
			isGUI = !isGUI;
		}
		break;
	}
}

void SPHScreen::Initialize()
{
	p.restDensity = 1000.f;
	p.restPressure = 0.001f;
	p.viscocity = 50.f;
	p.dt = 0.001f;
	p.stiffness = 20.f;//0.3;//3.5;
	p.surfaceTension = 0.8f;
	p.avgKernelParticles = 40;
	p.maxAcc = 100.f;
	p.maxVel = 0.8f;
	p.tensionTreshold = sqrtf(p.restDensity / p.avgKernelParticles);
	p.particleMass = 0.02f;
	p.particlesCount = 30000;// 65536;//32768;/*8192;*/ //10000;
	p.particleRadius = 2.f * sqrtf(p.particleMass * M_1_PI / p.restDensity);//*/std::cbrt(0.75 * p.particleMass * M_1_PI / p.restDensity);
	p.effectiveRadius = 3.f * sqrtf(p.avgKernelParticles * /*p.particleRadius * p.particleRadius*/ p.particleMass * M_1_PI / p.restDensity);//p.particleRadius * 3.5;*/std::cbrt(0.75 * p.particleMass * M_1_PI * p.avgKernelParticles / p.restDensity);
	//p.restDensity = 100000.f;

	BoundaryBox box =
	{
		p.particleRadius,
		width / scaleCoeff - p.particleRadius,
		p.particleRadius,
		height / scaleCoeff - p.particleRadius,
		0.,
		0.
	};

	fluid = new SPHFluid(p, box);
	
	dragParticle = false;
	fnt = new TextFont("..\\data\\Fonts\\arial\\arial.fnt");
	fnt->SetParamValue(glm::vec4(0.6, -6., 0.25, 34));

	sd.f = fluid;
	sd.p = &p;

	restDensitySlider = new Slider(200.f, height - 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	viscositySlider = new Slider(200.f, height - 2.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	stiffnessSlider = new Slider(200.f, height - 3.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	surfaceTensionSlider = new Slider(200.f, height - 4.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	massSlider = new Slider(200.f, height - 5.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	maxVelSlider = new Slider(200.f, height - 6.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	maxAccSlider = new Slider(200.f, height - 7.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));
	avgKernelParticlesSlider = new Slider(200.f, height - 8.f * 50.f, 400.f, 20.f, glm::vec4(1, 0, 0, 1));

	restDensitySlider->SetRange(0.017, 1000);
	viscositySlider->SetRange(0.001, 500);
	stiffnessSlider->SetRange(0.01, 200);
	surfaceTensionSlider->SetRange(0.001, 100);
	massSlider->SetRange(0.001, 3);
	maxVelSlider->SetRange(0.1, 100);
	maxAccSlider->SetRange(1, 200);
	avgKernelParticlesSlider->SetRange(1, 100);

	restDensitySlider->SetValue(p.restDensity);
	viscositySlider->SetValue(p.viscocity);
	stiffnessSlider->SetValue(p.stiffness);
	surfaceTensionSlider->SetValue(p.surfaceTension);
	massSlider->SetValue(p.particleMass);
	maxVelSlider->SetValue(p.maxVel);
	maxAccSlider->SetValue(p.maxAcc);
	avgKernelParticlesSlider->SetValue(p.avgKernelParticles);

	restDensitySlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->restDensity = value;
		//sd->p->particleRadius = sqrtf(sd->p->particleMass * M_1_PI / sd->p->restDensity);
		//sd->p->effectiveRadius = sqrtf(sd->p->avgKernelParticles * sd->p->particleRadius * sd->p->particleRadius);
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	viscositySlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->viscocity = value;
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	stiffnessSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->stiffness = value;
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	surfaceTensionSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->surfaceTension = value;
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	massSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->particleMass = value;
		sd->p->particleRadius = sqrtf(sd->p->particleMass * M_1_PI / sd->p->restDensity);
		sd->p->effectiveRadius = sqrtf(sd->p->avgKernelParticles * sd->p->particleRadius * sd->p->particleRadius);
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	maxVelSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->maxVel = value;
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	maxAccSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->maxAcc = value;
		sd->f->AdjustParams(*sd->p);
	}, &sd);
	avgKernelParticlesSlider->SetCallback(
		[](double value, void *data)
	{
		if (data == nullptr)
		{
			return;
		}
		ParamSliderData *sd = (ParamSliderData*)data;
		sd->p->avgKernelParticles = value;
		sd->p->tensionTreshold = sqrt(sd->p->restDensity / sd->p->avgKernelParticles);
		sd->p->effectiveRadius = sqrt(sd->p->avgKernelParticles * sd->p->particleRadius * sd->p->particleRadius);
		sd->f->AdjustParams(*sd->p);
	}, &sd);
}
