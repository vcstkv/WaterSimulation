#pragma once
#include "GUI/Screen.h"
#include "SPH/SPHFluid.h"

class SPHScreen :
	public Screen
{
public:
	SPHScreen(int width, int height);
	virtual ~SPHScreen();
	void Update(float delta);
	void Render(Graphics *graphics);
	void OnMouseButtonEvent(int btn, int action, int mods);
	void OnMouseCursorEvent(double x, double y);
	void OnKeyboardEvent(int key, int scanCode, int action, int mods);
	void OnKeyPress(int buttonID, bool isPressed);
	void OnKeyRelease(int buttonID);
	void Initialize();
private:
	SPHFluid *fluid;
	bool dragParticle;
	float scaleCoeff;
};

