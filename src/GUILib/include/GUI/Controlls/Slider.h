#pragma once
#include "glm/glm.hpp"
#include "Graphics/Graphics.h"

#ifdef GAMEENGINE_EXPORTS
#define GAMEENGINE_API __declspec(dllexport) 
#else
#define GAMEENGINE_API __declspec(dllimport)
#endif

typedef void (*SliderCb)(double value, void *cbData);

class Slider
{
public:
	GAMEENGINE_API Slider(float x, float y, float width, float heigth, glm::vec4 *color);
	GAMEENGINE_API ~Slider();
	GAMEENGINE_API void OnMouseButtonEvent(int btn, int action, int mods);
	GAMEENGINE_API void OnMouseCursorEvent(double x, double y);
	GAMEENGINE_API void Draw(Graphics *graphics, glm::mat4 *projection, glm::mat4 *view);
	GAMEENGINE_API void SetCallback(SliderCb cb, void *data);
	GAMEENGINE_API void SetRange(double mix, double max);
	GAMEENGINE_API void SetValue(double value);
	GAMEENGINE_API void SetStep(double step);
	GAMEENGINE_API double GetValue();

private:
	double value;
	int stepNum;
	int stepsCount;
	double step;
	double max;
	double min;
	glm::vec4 lineParams;
	glm::vec4 circleParams;
	glm::vec4 sliderParams;
	SliderCb cb;
	void *cbData;
	bool isPressed;
	glm::vec4 color;
	glm::dvec2 mousePos;
};

