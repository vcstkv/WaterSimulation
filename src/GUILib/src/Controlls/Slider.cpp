#include <cstdio>

#define GUILIB_EXPORT
#include "GUI/Controlls/Slider.h"

Slider::Slider(float x, float y, float width, float height, glm::vec4 *color)
{
	sliderParams = glm::vec4(x, y, width, height);
	lineParams = glm::vec4(x - width / 2.f + height / 2.f, y, x + width / 2.f - height / 2.f, y);
	circleParams = glm::vec4(lineParams.x, y, height / 2., height / 2.);
	this->color = *color;
	SetRange(1, 0);
	SetStep(0.1);
	isPressed = false;
}

Slider::~Slider()
{
}

void Slider::OnMouseButtonEvent(int btn, int action, int mods)
{
	if (btn == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			if(((mousePos.x - circleParams.x) * (mousePos.x - circleParams.x) + (mousePos.y - circleParams.y) * (mousePos.y - circleParams.y) < circleParams.z * circleParams.z))
			{
				isPressed = true;
			}
		}
		else
		{
			isPressed = false;
		}
	}
}

void Slider::OnMouseCursorEvent(double x, double y)
{
	mousePos.x = x;
	mousePos.y = y;
	if (isPressed)
	{
		circleParams.x = x;
		if (circleParams.x < lineParams.x)
		{
			circleParams.x = lineParams.x;
		}
		else if (circleParams.x > lineParams.z)
		{
			circleParams.x = lineParams.z;
		}
		value = min + (circleParams.x - lineParams.x) / (lineParams.z - lineParams.x) * (max - min);
		if (cb != nullptr)
		{
			cb(value, cbData);
		}
	}
}

void Slider::Draw(const std::shared_ptr<const Graphics> graphics, const glm::mat4 &projection, const glm::mat4 &view)
{
	graphics->DrawLine(glm::vec3(lineParams.x, lineParams.y, 0), glm::vec3(lineParams.z, lineParams.w, 0), 3, color, projection, view);
	graphics->DrawCircle(glm::vec3(circleParams.x, circleParams.y, 0), circleParams.z, circleParams.w, color, projection, view);
}

void Slider::SetCallback(SliderCb cb, void *data)
{
	if (cb == nullptr)
	{
		return;
	}
	this->cb = cb;
	this->cbData = data;
}

void Slider::SetRange(double min, double max)
{
	this->max = max;
	this->min = min;
	SetValue(min);
}

void Slider::SetValue(double value)
{
	this->value = value;
	circleParams.x = lineParams.x + value / (max - min) * (lineParams.z - lineParams.x);
	if (circleParams.x < lineParams.x)
	{
		circleParams.x = lineParams.x;
	}
	else if (circleParams.x > lineParams.z)
	{
		circleParams.x = lineParams.z;
	}
}

void Slider::SetStep(double step)
{
	this->step = step;
	stepsCount = static_cast<int>(round((max - min) / step));
}

double Slider::GetValue()
{
	return value;
}
