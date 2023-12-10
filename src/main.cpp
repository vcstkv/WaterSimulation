#include "GUI/Application.h"
#include "Screens/SPHScreen.h"

int main() {
    int screenWidth = 1280;
    int screenHeight = 900;

    Application a{"Water Simulation", screenWidth, screenHeight};
    a.Run(std::make_unique<SPHScreen>(screenWidth, screenHeight));
    return 0;
}
