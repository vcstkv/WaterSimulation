#include <iostream>
#include <vector>
#include "Screens/MainScreen.h"
#include "Screens/SPHScreen.h"
#include "GUI/ScreenController.h"

void main()
{
	ScreenController *screenController;
	int screenWidth = 800;
	int screenHeight = 700;
	screenController = new ScreenController(screenWidth, screenHeight, &std::string("Water Simulation"));
	screenController->AddScreen(new SPHScreen(screenWidth, screenHeight));
	screenController->StartMainLoop();
	delete screenController;
}



