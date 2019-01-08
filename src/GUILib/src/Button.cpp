#define GAMEENGINE_EXPORTS
#include "GUI/Button.h"




Button::Button(int x, int y, int width, int heigth)
{
	isPressed = false;
	sprite = new CommonSprite(x, y);
	sprite->SetSize(width, heigth);
	sprite->SetTexture("btn.png");
	sprite->SetShader("CommonSprite.vs", "CommonSprite.fs");

	pressedSprite = new CommonSprite(x, y);
	pressedSprite->SetSize(width, heigth);
	pressedSprite->SetTexture("btnPressed.png");
	pressedSprite->SetShader("CommonSprite.vs", "CommonSprite.fs");

	text = L"";
}


Button::~Button()
{
	delete sprite;
}

void Button::SetText(wchar_t *text)
{
	this->text = text;
}

void Button::Press()
{
	isPressed = true;
	if(OnPressed != nullptr) OnPressed();
}

void Button::Release()
{
	isPressed = false;
}

void Button::Draw(glm::mat4 *projection, glm::mat4 *view)
{
	if(isPressed)
		pressedSprite->Draw(projection, view);
	else
		sprite->Draw(projection, view);
}

void Button::SetCallback(void (*OnPressedFunc)())
{
	OnPressed = OnPressedFunc;
}
