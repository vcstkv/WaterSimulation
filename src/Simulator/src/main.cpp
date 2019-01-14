#include <iostream>
#include <vector>
#include "Screens/MainScreen.h"
#include "Screens/SPHScreen.h"
#include "GUI/ScreenController.h"

void main()
{
	ScreenController *screenController;
	int screenWidth = 1280;
	int screenHeight = 900;
	screenController = new ScreenController(screenWidth, screenHeight, &std::string("Water Simulation"));
	screenController->AddScreen(new SPHScreen(screenWidth, screenHeight));
	screenController->StartMainLoop();
	delete screenController;
}



