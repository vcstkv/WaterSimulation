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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindow = glfwCreateWindow(width, height, title->c_str(), NULL, NULL);
	if (glfwWindow == NULL)
	{
		fprintf(stderr, "Window create error");
		glfwTerminate();
		return nullptr;
	}

	glfwSwapInterval(0);

	glfwMakeContextCurrent(glfwWindow);

	glfwSetInputMode(glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(glfwWindow, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

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

void Window::PollEvents()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
}

bool Window::IsKeyPressed(int keyCode)
{
	return glfwGetKey(glfwWindow, keyCode) == GLFW_PRESS;
}

void Window::SetMouseButtonEventCb(MouseButtonEventCb cb, void *data)
{

	eventSorage.mcb = cb;
	eventSorage.mdata = data;
	glfwSetWindowUserPointer(glfwWindow, &eventSorage);
	glfwSetMouseButtonCallback(glfwWindow,
	[](GLFWwindow *w, int btn, int action, int mods)
	{
		GLFWCallbackEventStorage *s = static_cast<GLFWCallbackEventStorage*>(glfwGetWindowUserPointer(w));
		if (!s || !s->mcb)
		{
			return;
		}
		s->mcb(btn, action, mods, s->mdata);
	});
}

void Window::SetMouseCursorEventCb(MouseCursorEventCb cb, void *data)
{
	eventSorage.ccb = cb;
	eventSorage.cdata = data;
	glfwSetWindowUserPointer(glfwWindow, &eventSorage);
	glfwSetCursorPosCallback(glfwWindow,
	[](GLFWwindow *w, double x, double y)
	{
		GLFWCallbackEventStorage *s = static_cast<GLFWCallbackEventStorage*>(glfwGetWindowUserPointer(w));
		if (!s || !s->ccb)
		{
			return;
		}
		s->ccb(x, y, s->cdata);
	});
}

void Window::SetKeyboardEventCb(KeyboardEventCb cb, void * data)
{
	eventSorage.kcb = cb;
	eventSorage.kdata = data;
	glfwSetWindowUserPointer(glfwWindow, &eventSorage);
	glfwSetKeyCallback(glfwWindow,
	[](GLFWwindow *w, int key, int scanCode, int action, int mods)
	{
		GLFWCallbackEventStorage *s = static_cast<GLFWCallbackEventStorage*>(glfwGetWindowUserPointer(w));
		if (!s || !s->kcb)
		{
			return;
		}
		s->kcb(key, scanCode, action, mods, s->kdata);
	});
}


	
