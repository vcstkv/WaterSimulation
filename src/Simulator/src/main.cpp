#include "Screens/MainScreen.h"
#include "Screens/SPHScreen.h"
#include "GUI/Application.h"

void main()
{
	int screenWidth = 1280;
	int screenHeight = 900;
	std::string title{ "Water Simulation" };

	Application a{ std::move(title), screenWidth, screenHeight };
	a.Run(std::make_unique<MainScreen>(screenWidth, screenHeight));
}



