#define GUILIB_EXPORT
#include "GUI/Application.h"

Application::Application(std::string title, int width, int height)
	:
	title(std::move(title)),
	width(width),
	height(height),
	wnd(std::make_unique<Window>(title, width, height)),
	graphics(std::make_shared<Graphics>()),
	screenCtrl(std::make_unique<ScreenController>())
{
	wnd->AddInputEventsListener(screenCtrl.get());
}

Application::~Application()
{
}

void Application::Run(std::unique_ptr<Screen> mainScreen)
{
	screenCtrl->AddScreen(std::move(mainScreen));
	double lastTime = 0;
	double delta = 0;
	int FPS = 60;
	long constTimeTick = 1. / FPS;
	do
	{
		//OnKeyboardEvent();
		//std::this_thread::sleep_for(std::chrono::microseconds(constTimeTick));
		delta = wnd->GetTime() - lastTime;

		lastTime = delta + lastTime;

		screenCtrl->UpdateScreen(delta);

		screenCtrl->DrawScreen(graphics);

		wnd->PollEvents();

		wnd->SwapBuffers();

		//printf("\nFPS: %f", delta);

	} while (wnd->IsWindowShouldClose());
}
