#ifndef GUI_WINDOW_HEADER
#define GUI_WINDOW_HEADER

#include <string>
#include <list>

#include "glfw3/glfw3.h"
#include "GUI/InputEventsListener.h"
#include "GUILib.h"

class Window
{
public:
	GUILIB_API Window(const std::string &title, int width, int height);
	GUILIB_API ~Window();
	GUILIB_API double GetTime();
	GUILIB_API bool IsWindowShouldClose();
	GUILIB_API void PollEvents();
	GUILIB_API void SwapBuffers();
	GUILIB_API void AddInputEventsListener(InputEventsListener *listener);
	GUILIB_API void RemoveInputEventsListener(InputEventsListener *listener);
private:
	void SubscribeToGLFWInputEvents();
	GLFWwindow *glfwWindow = nullptr;
	std::list<InputEventsListener*> inputEventsListeners;
};

#endif //GUI_WINDOW_HEADER