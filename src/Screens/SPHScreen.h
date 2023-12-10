#pragma once

#include "GUI/Screen.h"
#include "SPH/SPHFluid.h"

class SPHScreen : public Screen {
public:
    SPHScreen(int width, int height);
    ~SPHScreen();
    void Update(float delta) override;
    void Render(const Graphics& graphics) override;
    void OnMouseButtonEvent(int btn, int action, int mods) override;
    void OnMouseCursorEvent(float x, float y) override;
    void OnMouseScrollEvent(float offsetX, float offsetY) override;
    void OnKeyboardEvent(int btn, int scanCode, int action, int mods) override;
    void Initialize() override;

private:
    SPHFluid* fluid{};
    bool dragParticle{false};
    float scaleCoeff{1.f};
};
