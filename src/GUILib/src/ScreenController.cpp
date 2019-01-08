#define GAMEENGINE_EXPORTS
#include "GUI/ScreenController.h"
#include "GUI/Screen.h"


ScreenController::ScreenController(int screenWidth, int screenHeight, std::string *title)
{
	screens = new std::vector<Screen*>();
	//Screen::SetScreenController(this);
	window = Window::Create(screenWidth, screenHeight, title);
	if (window == nullptr)
	{
		system("pause");
		exit(1);
	}

	//glfwSetMouseButtonCallback(window->GetGLFW(), &OnMouseEvent);

	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
	this->title = *title;
	graphics = new Graphics();
	graphics->SetScreenSize(screenWidth, screenHeight);
}



ScreenController::~ScreenController()
{
	for (int i = 0; i < screens->size(); i++)
	{
		delete (*screens)[i];
	}
	delete screens;
	delete window;
	delete graphics;
}

void ScreenController::StartMainLoop()
{
	double lastTime = 0;
	float delta = 0;
	int FPS = 60;
	double constTimeTick = 1. / FPS;
	do
	{

		OnKeyboardEvent();
		while (window->GetTime() - lastTime < constTimeTick) {}
		delta = window->GetTime() - lastTime;

		lastTime = window->GetTime();

		UpdateScreen(delta);

		DrawScreen(graphics);

		window->PollEvents();	

		//printf("\nFPS: %f", delta);
		

	} while (window->IsWindowShouldClose());
}

//void OnMouseEvent(GLFWwindow *window, int button, int action, int mods)
//{
//	printf("mouseCallBack");
//}

void ScreenController::OnKeyboardEvent()
{
	for (unsigned int i = GLFW_KEY_SPACE; i < GLFW_KEY_MENU; i++)
	{
		if (window->IsKeyPressed(i))
		{
			isKeyPressed = false;
			for (unsigned int j = 0; j < pressedKeysId.size(); j++)
			{
				if (i == pressedKeysId[j])
				{
					isKeyPressed = true;
				}
			}
			if (!isKeyPressed)
			{
				pressedKeysId.push_back(i);
			}
			screens->back()->OnKeyPress(i, isKeyPressed);
		}
	}
	for (unsigned int i = 0; i < pressedKeysId.size(); i++)
	{
		if (!window->IsKeyPressed(pressedKeysId[i]))
		{
			screens->back()->OnKeyRelease(pressedKeysId[i]);
			pressedKeysId.erase(pressedKeysId.begin() + i);
		}
	}
}

int ScreenController::AddScreen(Screen *screen)
{
	screens->push_back(screen);
	screen->SetScreenController(this);
	screen->Initialize();
	return screens->size() - 1;
}

void ScreenController::PreviousScreen()
{
	delete screens->back();
	screens->pop_back();
}

void ScreenController::DrawScreen(Graphics *graphics)
{
	screens->back()->Render(graphics);
}

void ScreenController::UpdateScreen(float delta)
{
	screens->back()->Update(delta);
}
