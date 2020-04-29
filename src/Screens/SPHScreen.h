#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GUI/Screen.h"
#include "SPH/SPHFluid.h"

class SPHScreen :
	public Screen
{
public:
	SPHScreen(int width, int height);
	~SPHScreen() override;
	void Update(float delta) override;
	void Render(const std::shared_ptr<const Graphics> graphics) override;
	void OnMouseButtonEvent(int btn, int action, int mods) override;
	void OnMouseCursorEvent(double x, double y) override;
	void OnMouseScrollEvent(double offsetX, double offsetY) override;
	void OnKeyboardEvent(int btn, int scanCode, int action, int mods) override;
	void Initialize();
private:
	SPHFluid *fluid;
	bool dragParticle;
	float scaleCoeff;
};

