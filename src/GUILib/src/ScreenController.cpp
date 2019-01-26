#define GAMEENGINE_EXPORTS
#include "GUI/ScreenController.h"
#include "GUI/Screen.h"
#include <thread>
#include <chrono>


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

	window->SetMouseButtonEventCb(
	[](int btn, int action, int mods, void *data)
	{
		std::vector<Screen*> *s = static_cast<std::vector<Screen*>*>(data);
		if (!s || s->empty())
		{
			return;
		}
		s->back()->OnMouseButtonEvent(btn, action, mods);
	}, screens);

	window->SetMouseCursorEventCb(
	[](double x, double y, void *data)
	{
		std::vector<Screen*> *s = static_cast<std::vector<Screen*>*>(data);
		if (!s || s->empty())
		{
			return;
		}
		s->back()->OnMouseCursorEvent(x, y);
	}, screens);

	window->SetKeyboardEventCb(
	[](int btn, int scanCode, int action, int mods, void *data)
	{
		std::vector<Screen*> *s = static_cast<std::vector<Screen*>*>(data);
		if (!s || s->empty())
		{
			return;
		}
		s->back()->OnKeyboardEvent(btn, scanCode, action, mods);
	}, screens);

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
	double delta = 0;
	int FPS = 60;
	long constTimeTick = 1. / FPS;
	do
	{
		//OnKeyboardEvent();
		//std::this_thread::sleep_for(std::chrono::microseconds(constTimeTick));
		delta = window->GetTime() - lastTime;

		lastTime = delta + lastTime;

		UpdateScreen(delta);

		DrawScreen(graphics);

		window->PollEvents();	

		//printf("\nFPS: %f", delta);
		
	} while (window->IsWindowShouldClose());
}

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

void ScreenController::UpdateScreen(double delta)
{
	screens->back()->Update(delta);
}
