#ifndef GUI_BUTTON_HEADER
#define GUI_BUTTON_HEADER

#include "Graphics/Shapes/Sprites/CommonSprite.h"
#include "glm/glm.hpp"
#include "GUILib.h"

enum class ButtonType
{
	Pictured,
	Colored
};

typedef void (*OnBtnPressedCb)();

class Button
{
public:
	GUILIB_API Button(int x, int y, int width, int heigth);
	GUILIB_API ~Button();
	GUILIB_API void Press();
	GUILIB_API void Release();
	GUILIB_API void SetText(wchar_t *text);
	GUILIB_API void Draw(const glm::mat4 &projection, const glm::mat4 &view);
	GUILIB_API void SetCallback(OnBtnPressedCb cb);

private:
	ButtonType type = ButtonType::Colored;
	wchar_t *text = L"";
	OnBtnPressedCb onPressedCb = nullptr;
	CommonSprite *sprite = nullptr;
	CommonSprite *pressedSprite = nullptr;
	bool isPressed = false;
};

#endif // GUI_BUTTON_HEADER