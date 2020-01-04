#define GUILIB_EXPORT
#include "GUI/ScreenController.h"

#include "GUI/Screen.h"

ScreenController::ScreenController()
{
}

ScreenController::~ScreenController()
{
}

void ScreenController::OnMouseScrollEvent(double offsetX, double offsetY)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->OnMouseScrollEvent(offsetX, offsetY);
}

void ScreenController::OnMouseButtonEvent(int btn, int action, int mods)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->OnMouseButtonEvent(btn, action, mods);
}

void ScreenController::OnMouseCursorEvent(double x, double y)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->OnMouseCursorEvent(x, y);
}

void ScreenController::OnKeyboardEvent(int btn, int scanCode, int action, int mods)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->OnKeyboardEvent(btn, scanCode, action, mods);
}

int ScreenController::AddScreen(std::unique_ptr<Screen> screen)
{
	screens.emplace(std::move(screen));
	screens.top()->SetScreenController(this);
	screens.top()->Initialize();
	return screens.size() - 1;
}

void ScreenController::PreviousScreen()
{
	if (screens.empty())
	{
		return;
	}
	screens.pop();
}

void ScreenController::DrawScreen(const std::shared_ptr<const Graphics> graphics)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->Render(graphics);
}

void ScreenController::UpdateScreen(float delta)
{
	if (screens.empty())
	{
		return;
	}
	screens.top()->Update(delta);
}
