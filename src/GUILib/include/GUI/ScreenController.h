#pragma once
#include "Graphics/Graphics.h"
#include "Window.h"
#include <vector>
#include <string>

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport) 
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif


class Screen;

class ScreenController
{
public:
	GAMEENGINE_API ScreenController(int screenWidth, int screenHeight, std::string *title);
	GAMEENGINE_API ~ScreenController();
	GAMEENGINE_API int AddScreen(Screen *screen);
	GAMEENGINE_API void PreviousScreen();
	GAMEENGINE_API void StartMainLoop();

private:
	void DrawScreen(Graphics *graphics);
	void UpdateScreen(float delta);
	std::vector<Screen*> *screens;
	void OnKeyboardEvent();
	//friend void OnMouseEvent(GLFWwindow *window, int button, int action, int mods);
	Window *window;
	int screenWidth;
	int screenHeight;
	std::string title;
	Graphics *graphics;
	std::vector<int> pressedKeysId;
	bool isKeyPressed;
};

