#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GUI/Screen.h"
#include "GUI/Controlls/Button.h"

constexpr int keysStates = GLFW_KEY_LAST;
constexpr int mouseStates = GLFW_MOUSE_BUTTON_LAST;

class MainScreen :
	public Screen
{
public:
	MainScreen(int width, int height);
	~MainScreen() override;
	void Update(float delta) override;
	void Render(const std::shared_ptr<const Graphics> graphics) override;
	void OnMouseButtonEvent(int btn, int action, int mods) override;
	void OnMouseCursorEvent(double x, double y) override;
	void OnMouseScrollEvent(double offsetX, double offsetY) override;
	void OnKeyboardEvent(int btn, int scanCode, int action, int mods) override;
	void Initialize();
private:
	Button *btn;
	bool keysState[keysStates] = { 0 };
	bool keysBlock[keysStates] = { 0 };
	bool mouseState[mouseStates] = { 0 };
	glm::vec2 mouse{0.f, 0.f};
	glm::vec2 angleCoeff{ 0.f, 0.f };
	glm::mat4 guiProjection;
};

