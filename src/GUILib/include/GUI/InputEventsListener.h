#ifndef GUI_INPUT_EVENTS_LISTENER_HEADER
#define GUI_INPUT_EVENTS_LISTENER_HEADER

struct InputEventsListener
{
	virtual ~InputEventsListener() {}
	virtual void OnMouseButtonEvent(int btn, int action, int mods) = 0;
	virtual void OnMouseCursorEvent(double x, double y) = 0;
	virtual void OnMouseScrollEvent(double offsetX, double offsetY) = 0;
	virtual void OnKeyboardEvent(int btn, int scanCode, int action, int mods) = 0;
};

#endif //GUI_INPUT_EVENTS_LISTENER_HEADER