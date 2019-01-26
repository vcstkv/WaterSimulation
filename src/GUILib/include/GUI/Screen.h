#pragma once
#include "Graphics/Graphics.h"
#include "ScreenController.h"

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport) 
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif

class ScreenController;

class Screen
{
public:
	GAMEENGINE_API Screen(int width, int height);
	GAMEENGINE_API virtual ~Screen();
	GAMEENGINE_API virtual void Update(float delta) = 0;
	GAMEENGINE_API virtual void Render(Graphics *graphics) = 0;
	GAMEENGINE_API virtual void OnMouseButtonEvent(int btn, int action, int mods) = 0;
	GAMEENGINE_API virtual void OnMouseCursorEvent(double x, double y) = 0;
	GAMEENGINE_API virtual void OnKeyboardEvent(int key, int scanCode, int action, int mods) = 0;
	GAMEENGINE_API virtual void OnKeyPress(int buttonID, bool isPressed) = 0;
	GAMEENGINE_API virtual void OnKeyRelease(int buttonID) = 0;
	GAMEENGINE_API virtual void Initialize() = 0;
	GAMEENGINE_API void SetScreenController(ScreenController *screenController);
	int width;
	int height;
protected:
	ScreenController *screenController;
	glm::mat4 projection;
	glm::mat4 view;
};

