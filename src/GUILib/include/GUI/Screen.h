#ifndef GUI_SCREEN_HEADER
#define GUI_SCREEN_HEADER

#include "Graphics/Graphics.h"
#include "GUI/InputEventsListener.h"
#include "GUILib.h"

class ScreenController;

class Screen : public InputEventsListener
{
public:
	GUILIB_API Screen(int width, int height);
	GUILIB_API virtual ~Screen();
	GUILIB_API virtual void Update(float delta) = 0;
	GUILIB_API virtual void Render(const std::shared_ptr<const Graphics> graphics) = 0;
	GUILIB_API virtual void Initialize() = 0;
	GUILIB_API void SetScreenController(ScreenController *screenController);
	int width = 800;
	int height = 600;
protected:
	ScreenController *screenController;
	glm::mat4 projection;
	glm::mat4 view;
};

#endif //GUI_SCREEN_HEADER