#pragma once
#include "glfw3/glfw3.h"
#include <string>
#include <vector>

class Window
{
public:
	static Window* Create(int width, int height, std::string *title);
	double GetTime();
	bool IsWindowShouldClose();
	void PollEvents();
	GLFWwindow* GetGLFW();
	bool IsKeyPressed(int keyCode);
	~Window();
private:
	GLFWwindow *glfwWindow;
	std::string title;
	int width;
	int height;
	Window(GLFWwindow *glfwWindow, int width, int height, std::string *title);
};

