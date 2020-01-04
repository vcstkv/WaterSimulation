#ifndef GUI_APPLICATION_HEADER
#define GUI_APPLICATION_HEADER

#include <string>

#include "Graphics/Graphics.h"
#include "GUI/Window.h"
#include "GUI/ScreenController.h"
#include "GUI/Screen.h"
#include "GUILib.h"

class Application
{
public:
	GUILIB_API Application(std::string title, int width, int height);
	GUILIB_API ~Application();
	GUILIB_API void Run(std::unique_ptr<Screen> mainScreen);
private:
	std::string title;
	int width = 800;
	int height = 600;
	std::unique_ptr<Window> wnd;
	std::shared_ptr<Graphics> graphics;
	std::unique_ptr<ScreenController> screenCtrl;
};

#endif //GUI_APPLICATION_HEADER