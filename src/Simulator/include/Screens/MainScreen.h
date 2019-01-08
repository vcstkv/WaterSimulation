#pragma once
#include "GUI/Screen.h"
#include "GUI/Button.h"

class MainScreen :
	public Screen
{
public:
	MainScreen(int width, int height);
	virtual ~MainScreen();
	void Update(float delta);
	void Render(Graphics *graphics);
	void OnKeyPress(int buttonID, bool isPressed);
	void OnKeyRelease(int buttonID);
	void Initialize();
	Button *btn;
};

