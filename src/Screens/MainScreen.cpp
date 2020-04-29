#include "Screens/MainScreen.h"

#include <iostream>

#include "Graphics/Text/TextFont.h"
#include "Graphics/Shapes/Sprites/CommonSprite.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

int textSize;
TextFont *font;
CommonSprite *grass;

const glm::vec3 up = glm::vec3(0., 1., 0.);

struct Camera
{
	glm::vec3 p;
	glm::vec3 d;
	glm::vec3 u;
	glm::vec3 r;
	static constexpr float v = 20;
};

void TestFunc()
{
	printf("TEST CALLBACK\n");
}

Camera cam;

glm::vec3 d;
glm::vec3 u;
glm::vec3 r;

MainScreen::MainScreen(int width, int height)
	:
	Screen(width, height)
{
	glClearColor(.1f, .67f, 1.f, 1.f);

	projection = glm::perspectiveRH_NO(45.f, float(width) / height, 0.1f, 100.f);
	guiProjection = glm::ortho(0.f, 1.f * width, 0.f, 1.f * height, -1.f, 1.f);

	textSize = 36;
	font = new TextFont("../data/Fonts/arial/arial.fnt");
	font->SetParamValue(glm::vec4(0.6, -6., 0.25, 34));

	btn = new Button(width - 60, 400, 10, 100);
	btn->SetCallback(&TestFunc);
	btn->Press();
	btn->Release();

	grass = new CommonSprite(glm::vec3(0.f));
	grass->SetTexture("bg/grass.png");
	grass->SetShader("CommonSprite.vs", "CommonSprite.fs");
	grass->SetSize(1.f, 1.f);

	cam.p = glm::vec3(0., 1., 0.);
	cam.d = glm::vec3(1., 0., 0.);
	cam.u = glm::vec3(0., 1., 0.);
	cam.r = glm::normalize(glm::cross(cam.d, cam.u));

	d = cam.d;
	r = cam.r;
	u = cam.u;

	angleCoeff.x = 0.5f * glm::two_pi<float>() / width;
	angleCoeff.y = 0.5f * glm::two_pi<float>() / height;
}

MainScreen::~MainScreen()
{
}

static int fps = 0;

void MainScreen::Update(float delta)
{
    if (delta != 0.f)
    {
        fps = static_cast<int>(roundf(1.f/delta));
    }

	if (keysState[GLFW_KEY_W])
	{
		cam.p += cam.d * cam.v * delta;
	}
	if (keysState[GLFW_KEY_S])
	{
		cam.p -= cam.d * cam.v * delta;
	}
	if (keysState[GLFW_KEY_D])
	{
		cam.p += cam.r * cam.v * delta;
	}
	if (keysState[GLFW_KEY_A])
	{
		cam.p -= cam.r * cam.v * delta;
	}
	if (keysState[GLFW_KEY_Q])
	{
		cam.d = cam.d * glm::angleAxis(-glm::pi<float>() * 0.5f * delta, cam.u);
	}
	if (keysState[GLFW_KEY_E])
	{
		cam.d = cam.d * glm::angleAxis(glm::pi<float>() * 0.5f * delta, cam.u);
	}
	if (keysState[GLFW_KEY_SPACE])
	{
		cam.p += cam.u * cam.v * delta;
        //grass->Rotate(glm::vec3(1, 1, 0), -0.2 * delta);
	}
	if (keysState[GLFW_KEY_KP_ADD])
    {
	    grass->Scale(glm::vec3(powf(1.5f, delta), powf(1.5f, delta), 1.f));
    }
    if (keysState[GLFW_KEY_KP_SUBTRACT])
    {
        grass->Scale(glm::vec3(powf(0.6f, delta), powf(0.6f, delta), 1.f));
    }
    if (keysState[GLFW_KEY_UP])
    {
        grass->Translate(0.f, 0.f, cam.v * delta);
    }
    if (keysState[GLFW_KEY_DOWN])
    {
        grass->Translate(0.f, 0.f, -cam.v * delta);
    }
    if (keysState[GLFW_KEY_LEFT])
    {
        grass->Translate(-cam.v * delta, 0.f, 0.f);
    }
    if (keysState[GLFW_KEY_RIGHT])
    {
        grass->Translate(cam.v * delta, 0.f, 0.f);
    }

	view = glm::lookAtRH(cam.p, cam.p + cam.d, cam.u);
}

void MainScreen::Render(const std::shared_ptr<const Graphics> graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            grass->SetPosition(i, 0, j);
            grass->SetAngle(glm::vec3(1, 0, 0), glm::half_pi<float>());
            grass->Draw(projection, view);
        }
    }
	font->DrawText(L"fps: " + std::to_wstring(fps), textSize, glm::vec4(0, 0, 0, 1), 0, height, guiProjection);
	font->DrawText(L"pos: " + std::to_wstring(cam.p.x) + L"; " + std::to_wstring(cam.p.y) + L"; " + std::to_wstring(cam.p.z), textSize, glm::vec4(0, 0, 0, 1), 0, 600, guiProjection);
	btn->Draw(guiProjection, glm::mat4(1));
	glm::vec3 p(0., 0., 0.);
	graphics->DrawLine(p, p + u, 2.f, glm::vec4(1, 0, 0, 1), projection, view);
	graphics->DrawLine(p, p + d, 2.f, glm::vec4(0, 1, 0, 1), projection, view);
	graphics->DrawLine(p, p + r, 2.f, glm::vec4(1, 1, 0, 1), projection, view);
	//std::cout << glGetError() << std::endl;
}

void MainScreen::OnMouseButtonEvent(int btn, int action, int mods)
{
	mouseState[btn] = action;
}

void MainScreen::OnMouseCursorEvent(double x, double y)
{
	if (mouseState[GLFW_MOUSE_BUTTON_LEFT])
	{
		cam.r = glm::normalize(glm::cross(cam.d, cam.u));
		glm::qua<float> q = glm::angleAxis((float(y) - mouse.y) * angleCoeff.y, cam.r)
			              * glm::angleAxis((float(x) - mouse.x) * angleCoeff.x, up);
		cam.u = cam.u * q;
		cam.d = cam.d * q;
	}
	mouse.x = x;
	mouse.y = y;
}

void MainScreen::OnMouseScrollEvent(double offsetX, double offsetY)
{

}

void MainScreen::OnKeyboardEvent(int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN)
	{
		return;
	}

	keysState[key] = action;
}

void MainScreen::Initialize()
{

}
