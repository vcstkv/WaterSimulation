#pragma once
#include "Graphics/Shapes/Sprites/CommonSprite.h"
#include "glm/glm.hpp"

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport) 
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif

#define PICTURED_BUTTON 0
#define COLORED_BUTTON 1

class Button
{
public:
	GAMEENGINE_API Button(int x, int y, int width, int heigth);
	GAMEENGINE_API ~Button();
	GAMEENGINE_API void Press();
	GAMEENGINE_API void Release();
	GAMEENGINE_API void SetText(wchar_t *text);
	GAMEENGINE_API void Draw(glm::mat4 *projection, glm::mat4 *view);
	GAMEENGINE_API void SetCallback(void (*OnPressedFunc)());

private:
	uint8_t type;
	wchar_t *text;
	void (*OnPressed)();
	CommonSprite *sprite;
	CommonSprite *pressedSprite;
	bool isPressed;
};

