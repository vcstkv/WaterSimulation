#include "Screens/MainScreen.h"

#include <cmath>

#include "GUI/Controlls/Button.h"
#include "Graphics/Shapes/Model.h"
#include "Graphics/Shapes/Sprites/CommonSprite.h"
#include "Graphics/Text/TextFont.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"

void TestFunc(void* data) { printf("TEST CALLBACK\n"); }

MainScreen::~MainScreen() = default;

MainScreen::MainScreen(int width, int height) : Screen(width, height) {
    glClearColor(.1f, .67f, 1.f, 1.f);
    glEnable(GL_DEPTH_TEST);

    projection = glm::perspectiveRH_NO(45.f, static_cast<float>(width) / height, 0.1f, 100.f);
    guiProjection = glm::ortho(0.f, 1.f * width, 0.f, 1.f * height, -1.f, 1.f);

    textSize = 36;
    font = new TextFont("data/Fonts/arial/arial.fnt");
    font->SetParamValue(glm::vec4(0.6, -6., 0.25, 34));

    button = new Button(width - 60, 400, 10, 100);
    button->SetCallback(&TestFunc);
    button->Press();
    button->Release();

    grass = new CommonSprite(glm::vec3(0.f));
    grass->SetTexture("bg/grass.png");
    grass->SetShader("CommonSprite.vs", "CommonSprite.fs");
    grass->SetSize(10.f, 10.f);
    grass->Rotate(glm::vec3(1, 0, 0), glm::half_pi<float>());

    mesh = std::make_unique<Model>("data/Models/Camry/Toyota Camry .obj");
    meshModel2 = glm::translate(meshModel2, glm::vec3(1, 0, 0));
    // meshModel1 = glm::translate(meshModel1, glm::vec3(-1, 0, 0));

    cam.p = glm::vec3(0., 1., 0.);
    cam.d = glm::vec3(1., 0., 0.);
    cam.u = glm::vec3(0., 1., 0.);
    cam.r = glm::normalize(glm::cross(cam.d, cam.u));

    d = cam.d;
    r = cam.r;
    u = cam.u;

    angleCoeff.x = 0.5f * glm::two_pi<float>() / width;
    angleCoeff.y = 0.5f * glm::two_pi<float>() / height;
}

void MainScreen::Update(float delta) {
    if (delta != 0.f) {
        fps = static_cast<int>(std::round(1.f / delta));
    }

    if (keysState[GLFW_KEY_W]) {
        cam.p += cam.d * cam.v * delta;
    }
    if (keysState[GLFW_KEY_S]) {
        cam.p -= cam.d * cam.v * delta;
    }
    if (keysState[GLFW_KEY_D]) {
        cam.p += cam.r * cam.v * delta;
    }
    if (keysState[GLFW_KEY_A]) {
        cam.p -= cam.r * cam.v * delta;
    }
    if (keysState[GLFW_KEY_Q]) {
        cam.d = cam.d * glm::angleAxis(-glm::pi<float>() * 0.5f * delta, cam.u);
    }
    if (keysState[GLFW_KEY_E]) {
        cam.d = cam.d * glm::angleAxis(glm::pi<float>() * 0.5f * delta, cam.u);
    }
    if (keysState[GLFW_KEY_SPACE]) {
        cam.p += cam.u * cam.v * delta;
        // grass->Rotate(glm::vec3(1, 1, 0), -0.2 * delta);
    }
    if (keysState[GLFW_KEY_KP_ADD]) {
        grass->Scale(glm::vec3(powf(1.5f, delta), powf(1.5f, delta), 1.f));
    }
    if (keysState[GLFW_KEY_KP_SUBTRACT]) {
        grass->Scale(glm::vec3(powf(0.6f, delta), powf(0.6f, delta), 1.f));
    }
    if (keysState[GLFW_KEY_UP]) {
        grass->Translate(0.f, 0.f, cam.v * delta);
    }
    if (keysState[GLFW_KEY_DOWN]) {
        grass->Translate(0.f, 0.f, -cam.v * delta);
    }
    if (keysState[GLFW_KEY_LEFT]) {
        grass->Translate(-cam.v * delta, 0.f, 0.f);
    }
    if (keysState[GLFW_KEY_RIGHT]) {
        grass->Translate(cam.v * delta, 0.f, 0.f);
    }

    view = glm::lookAtRH(cam.p, cam.p + cam.d, cam.u);
}

void MainScreen::Render(const Graphics& graphics) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grass->Draw(projection, view);

    glm::vec3 p(0., 0., 0.);
    graphics.DrawLine(p, p + u, 2.f, glm::vec4(1, 0, 0, 1), projection, view);
    graphics.DrawLine(p, p + d, 2.f, glm::vec4(0, 1, 0, 1), projection, view);
    graphics.DrawLine(p, p + r, 2.f, glm::vec4(1, 1, 0, 1), projection, view);
    graphics.DrawLine(p, glm::vec3(2, 2, 2), 2.f, glm::vec4(1, 1, 1, 1), projection, view);
    mesh->Draw(meshModel1, projection, view, cam.p);

    font->DrawText(L"fps: " + std::to_wstring(fps), textSize, glm::vec4(0, 0, 0, 1), 0, height,
                   guiProjection);
    font->DrawText(L"pos: " + std::to_wstring(cam.p.x) + L"; " + std::to_wstring(cam.p.y) + L"; " +
                       std::to_wstring(cam.p.z),
                   textSize, glm::vec4(0, 0, 0, 1), 0, 600, guiProjection);
    button->Draw(guiProjection, glm::mat4(1));
    //	std::cout << glGetError() << std::endl;
}

void MainScreen::OnMouseButtonEvent(int btn, int action, int mods) { mouseState[btn] = action; }

void MainScreen::OnMouseCursorEvent(float x, float y) {
    if (mouseState[GLFW_MOUSE_BUTTON_LEFT]) {
        cam.r = glm::normalize(glm::cross(cam.d, cam.u));
        glm::qua<float> q = glm::angleAxis((y - mouse.y) * angleCoeff.y, cam.r) *
                            glm::angleAxis((x - mouse.x) * angleCoeff.x, up);
        cam.u = cam.u * q;
        cam.d = cam.d * q;
    }
    mouse.x = x;
    mouse.y = y;
}

void MainScreen::OnMouseScrollEvent(float offsetX, float offsetY) {}

void MainScreen::OnKeyboardEvent(int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) {
        return;
    }

    keysState[key] = action;
}

void MainScreen::Initialize() {}
