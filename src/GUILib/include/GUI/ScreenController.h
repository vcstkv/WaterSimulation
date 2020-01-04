#ifndef GUI_SCREEN_CONTROLLER_HEADER
#define GUI_SCREEN_CONTROLLER_HEADER

#include <stack>

#include "Graphics/Graphics.h"
#include "GUI/InputEventsListener.h"
#include "GUILib.h"

class Screen;

class ScreenController : public InputEventsListener
{
public:
	GUILIB_API ScreenController();
	GUILIB_API ~ScreenController() override;
	GUILIB_API int AddScreen(std::unique_ptr<Screen> screen);
	GUILIB_API void PreviousScreen();
	GUILIB_API void DrawScreen(const std::shared_ptr<const Graphics> graphics);
	GUILIB_API void UpdateScreen(float delta);
	GUILIB_API void OnMouseButtonEvent(int btn, int action, int mods) override;
	GUILIB_API void OnMouseCursorEvent(double x, double y) override;
	GUILIB_API void OnMouseScrollEvent(double offsetX, double offsetY) override;
	GUILIB_API void OnKeyboardEvent(int btn, int scanCode, int action, int mods) override;
private:
	std::stack<std::unique_ptr<Screen>> screens;
};

#endif //GUI_SCREEN_CONTROLLER_HEADER