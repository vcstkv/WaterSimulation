#include "GUI/Window.h"
#include <stdio.h>



Window::Window(GLFWwindow *glfwWindow, int width, int height, std::string *title)
{
	this->glfwWindow = glfwWindow;
	this->height = height;
	this->width = width;
	this->title = *title;
}

Window* Window::Create(int width, int height, std::string *title)
{
	GLFWwindow *glfwWindow;

	if (!glfwInit())
	{
		fprintf(stderr, "GLFW init error\n");
		return nullptr;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindow = glfwCreateWindow(width, height, title->c_str(), NULL, NULL);
	if (glfwWindow == NULL)
	{
		fprintf(stderr, "Window create error");
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(glfwWindow);

	glfwSetInputMode(glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);


	return new Window(glfwWindow, width, height, title);
}


Window::~Window()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

double Window::GetTime()
{
	return glfwGetTime();
}


bool Window::IsWindowShouldClose()
{
	return glfwWindowShouldClose(glfwWindow) == 0;
}

GLFWwindow* Window::GetGLFW()
{
	return glfwWindow;
}

void Window::PollEvents()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
}

bool Window::IsKeyPressed(int keyCode)
{
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}
	
