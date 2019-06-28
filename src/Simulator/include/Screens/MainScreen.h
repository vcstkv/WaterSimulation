#pragma once
#include "GUI/Screen.h"
#include "GUI/Controlls/Button.h"

class MainScreen :
	public Screen
{
public:
	MainScreen(int width, int height);
	virtual ~MainScreen();
	void Update(float delta);
	void Render(Graphics *graphics);
	void OnKeyPress(int buttonID, bool isPressed);
	void OnMouseButtonEvent(int btn, int action, int mods);
	void OnMouseCursorEvent(double x, double y);
	void OnKeyboardEvent(int key, int scanCode, int action, int mods);
	void OnKeyRelease(int buttonID);
	void Initialize();
	Button *btn;
};

