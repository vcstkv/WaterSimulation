#include "Screens/SPHScreen.h"


SPHScreen::~SPHScreen()
{
}

SPHScreen::SPHScreen(int width, int height) : Screen(width, height)
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	projection = glm::ortho(0.0f, width * 1.0f, 0.0f, height*1.0f, 1.0f, -1.0f);
	view = glm::mat4(1);
}

void SPHScreen::Update(float delta)
{

}

void SPHScreen::Render(Graphics *graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//std::cout << glGetError() << std::endl;
}

void SPHScreen::OnKeyPress(int buttonID, bool isPressed)
{
	switch (buttonID)
	{
	case GLFW_KEY_Q:
		if (!isPressed)
		{
		}
		break;
	case GLFW_KEY_W:
		if (!isPressed)
		{
		}
		break;
	case GLFW_KEY_E:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_R:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_A:
		if (!isPressed)
		{
		}
		break;
	case GLFW_KEY_S:
		if (!isPressed)
		{
		}
		break;
	case GLFW_KEY_D:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_F:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_EQUAL:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_MINUS:
		//if (!isPressed)
		//{
		//}
		break;
	case GLFW_KEY_B:
		if (!isPressed)
		{
		}
		break;
	}
}

void SPHScreen::OnKeyRelease(int buttonID)
{
	switch (buttonID)
	{
	case GLFW_KEY_B:
		break;
	}
}

void SPHScreen::Initialize()
{

}
