#ifndef GUI_SLIDER_HEADER
#define GUI_SLIDER_HEADER

#include "glm/glm.hpp"
#include "Graphics/Graphics.h"
#include "GUILib.h"

typedef void (*SliderCb)(double value, void *cbData);

class Slider
{
public:
	GUILIB_API Slider(float x, float y, float width, float heigth, glm::vec4 *color);
	GUILIB_API ~Slider();
	GUILIB_API void OnMouseButtonEvent(int btn, int action, int mods);
	GUILIB_API void OnMouseCursorEvent(double x, double y);
	GUILIB_API void Draw(const std::shared_ptr<const Graphics> graphics, const glm::mat4 &projection, const glm::mat4 &view);
	GUILIB_API void SetCallback(SliderCb cb, void *data);
	GUILIB_API void SetRange(double mix, double max);
	GUILIB_API void SetValue(double value);
	GUILIB_API void SetStep(double step);
	GUILIB_API double GetValue();

private:
	double value = 0.;
	int stepNum = 0;
	int stepsCount = 0;
	double step = 0.;
	double max = 0.;
	double min = 0.;
	glm::vec4 lineParams{ 0.f };
	glm::vec4 circleParams{ 0.f };
	glm::vec4 sliderParams{ 0.f };
	SliderCb cb = nullptr;
	void *cbData = nullptr;
	bool isPressed = false;
	glm::vec4 color{ 0.f };
	glm::dvec2 mousePos{ 0. };
};

#endif //GUI_SLIDER_HEADER