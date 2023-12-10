#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#include "GUI/Screen.h"


constexpr inline int keysStates = GLFW_KEY_LAST;
constexpr inline int mouseStates = GLFW_MOUSE_BUTTON_LAST;

struct Camera {
    glm::vec3 p;
    glm::vec3 d;
    glm::vec3 u;
    glm::vec3 r;
    float v = 20;
};

class Model;
class TextFont;
class Button;
class CommonSprite;

class MainScreen : public Screen {
public:
    MainScreen(int width, int height);
    ~MainScreen();
    void Update(float delta) override;
    void Render(const Graphics& graphics) override;
    void OnMouseButtonEvent(int btn, int action, int mods) override;
    void OnMouseCursorEvent(float x, float y) override;
    void OnMouseScrollEvent(float offsetX, float offsetY) override;
    void OnKeyboardEvent(int btn, int scanCode, int action, int mods) override;
    void Initialize() override;

private:
    Button* button{};
    bool keysState[keysStates] = {false};
    bool keysBlock[keysStates] = {false};
    bool mouseState[mouseStates] = {false};
    glm::vec2 mouse{0.f, 0.f};
    glm::vec2 angleCoeff{0.f, 0.f};
    glm::mat4 guiProjection{};
    glm::vec3 d{};
    glm::vec3 u{};
    glm::vec3 r{};
    Camera cam{};
    int textSize{};
    TextFont* font{};
    Sprite* grass{};
    std::unique_ptr<Model> mesh;
    glm::mat4 meshModel1{1};
    glm::mat4 meshModel2{1};
    const glm::vec3 up = glm::vec3(0., 1., 0.);
    int fps{};
};
