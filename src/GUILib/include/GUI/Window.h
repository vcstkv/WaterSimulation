#pragma once
#include "glfw3/glfw3.h"
#include <string>
#include <vector>

typedef void(*MouseButtonEventCb)(int, int, int, void *data);

typedef void(*KeyboardEventCb)(int, int, int, int, void *data);

typedef void(*MouseCursorEventCb)(double, double, void *data);

typedef struct {
	KeyboardEventCb kcb;
	void *kdata;
	MouseButtonEventCb mcb;
	void *mdata;
	MouseCursorEventCb ccb;
	void *cdata;
} GLFWCallbackEventStorage;

class Window
{
public:
	static Window* Create(int width, int height, std::string *title);
	double GetTime();
	bool IsWindowShouldClose();
	void PollEvents();
	bool IsKeyPressed(int keyCode);
	void SetMouseButtonEventCb(MouseButtonEventCb cb, void *data);
	void SetMouseCursorEventCb(MouseCursorEventCb cb, void *data);
	void SetKeyboardEventCb(KeyboardEventCb cb, void *data);
	~Window();
private:
	Window(GLFWwindow *glfwWindow, int width, int height, std::string *title);
	GLFWwindow *glfwWindow;
	std::string title;
	int width;
	int height;
	GLFWCallbackEventStorage eventSorage;
};

