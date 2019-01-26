#include "Screens/MainScreen.h"
#include "Graphics/Text/TextFont.h"
#include "Graphics/Shapes/Sprites/CommonSprite.h"

using namespace glm;
int textSize;
TextFont *font;
CommonSprite *grass;
float **grassArr;
int grassCountX;
int grassCountY;

void TestFunc()
{
	printf("\nTEST CALLBACK");
}

MainScreen::MainScreen(int width, int height) : Screen(width, height)
{
	glClearColor(.1f, .67f, 1.f, 1.f);
	projection = ortho(0.0f, width * 1.0f, 0.0f, height*1.0f, 1.0f, -1.0f);
	view = mat4(1);
	textSize = 36;
	font = new TextFont("..\\data\\Fonts\\arial\\arial.fnt");
	font->SetParamValue(&glm::vec4(0.6, -6., 0.25, 34));
	btn = new Button(400, 400, 10, 100);
	btn->SetCallback(&TestFunc);
	btn->Press();
	btn->Release();
	grass = new CommonSprite(0, 0);
	grass->SetTexture("bg\\grass.png");
	grass->SetShader("CommonSprite.vs", "CommonSprite.fs");
	grassCountX = width / grass->width + 1;
	grassCountY = height / grass->height + 1;
	//grassArr = new float[grassCountY * grassCountX];

}


MainScreen::~MainScreen()
{
}

void MainScreen::Update(float delta)
{

}

void MainScreen::Render(Graphics *graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < grassCountY; i++)
	{
		for (int j = 0; j < grassCountX; j++)
		{
			grass->SetPosition(j * grass->width + grass->width / 2, i * grass->height + grass->height / 2);
			grass->Draw(&projection, &view);
		}
	}
	graphics->DrawLine(400, 400, 0, 0, &vec4(1, 1, 0, 1), &projection);
	font->DrawText(L"Jdg12", textSize, &vec4(0, 0, 0, 1), 0, 600, &projection);
	btn->Draw(&projection, &view);
	//std::cout << glGetError() << std::endl;
}

void MainScreen::OnKeyPress(int buttonID, bool isPressed)
{
	switch (buttonID)
	{
	case GLFW_KEY_Q:
		if (!isPressed)
		{
			font->AddParamValue(&glm::vec4(-0.05, 0 ,0 ,0 ));
		}
		break;
	case GLFW_KEY_W:
		if (!isPressed)
		{
			font->AddParamValue(&glm::vec4(0.05, 0, 0, 0));
		}
		break;
	case GLFW_KEY_E:
		//if (!isPressed)
		//{
			font->AddParamValue(&glm::vec4(0, -1, 0, 0));
		//}
		break;
	case GLFW_KEY_R:
		//if (!isPressed)
		//{
			font->AddParamValue(&glm::vec4(0, 1, 0, 0));
		//}
		break;
	case GLFW_KEY_A:
		if (!isPressed)
		{
			font->AddParamValue(&glm::vec4(0, 0, -0.05, 0));
		}
		break;
	case GLFW_KEY_S:
		if (!isPressed)
		{
			font->AddParamValue(&glm::vec4(0, 0, 0.05, 0));
		}
		break;
	case GLFW_KEY_D:
		//if (!isPressed)
		//{
			font->AddParamValue(&glm::vec4(0, 0, 0, -1));
		//}
		break;
	case GLFW_KEY_F:
		//if (!isPressed)
		//{
			font->AddParamValue(&glm::vec4(0, 0, 0, 1));
		//}
		break;
	case GLFW_KEY_EQUAL:
		//if (!isPressed)
		//{
			textSize++;
		//}
		break;
	case GLFW_KEY_MINUS:
		//if (!isPressed)
		//{
			textSize--;
		//}
		break;
	case GLFW_KEY_B:
		if (!isPressed)
		{
			btn->Press();
		}
		break;
	}
}

void MainScreen::OnMouseButtonEvent(int btn, int action, int mods)
{
}

void MainScreen::OnMouseCursorEvent(double x, double y)
{
}

void MainScreen::OnKeyboardEvent(int key, int scanCode, int action, int mods)
{
}

void MainScreen::OnKeyRelease(int buttonID)
{
	switch (buttonID)
	{
	case GLFW_KEY_B:
			btn->Release();
		break;
	}
}

void MainScreen::Initialize()
{

}
