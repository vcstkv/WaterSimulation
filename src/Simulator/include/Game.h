#pragma once
#ifdef GAMEENGINE_EXPORTS
#define GAME_API __declspec(dllexport) 
#else
#define GAME_API __declspec(dllimport)
#endif
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "Graphics/Graphics.h"

class Game
{
public:
	GAME_API Game(int width, int height);
	GAME_API ~Game();
	GAME_API void Update(float delta);
	GAME_API void Render();
	GAME_API void OnKeyPress(int buttonID, bool isPressed);
	GAME_API void OnKeyRelease(int buttonID);
	
};

