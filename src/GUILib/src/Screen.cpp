#define GUILIB_EXPORT
#include "GUI/Screen.h"
#include "GUI/ScreenController.h"

Screen::Screen(int width, int height) :
	width(width),
	height(height),
	screenController(nullptr),
	view(1)
{}


Screen::~Screen()
{}

void Screen::SetScreenController(ScreenController *screenController)
{ 
	Screen::screenController = screenController; 
}
