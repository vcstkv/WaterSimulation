#pragma once

#include "GLHeaders.h"
#include "Shaders/SpriteShaderProgram.h"
#include "Textures/TGALoader.h"
#include "Shapes/Line.h"
#include "Shapes/Circle.h"
#include "Shapes/Sprites/Sprite.h"
#include "Textures/TexturesController.h"
#include "GraphicsResources.h"

class Graphics
{
public:
	GRAPHICS_API Graphics();
	GRAPHICS_API ~Graphics();
	GRAPHICS_API void DrawLine(const glm::vec3 &b, const glm::vec3 &e, float width, const glm::vec4 &color, const glm::mat4 &projection, const glm::mat4 &view) const;
	GRAPHICS_API void DrawCircle(const glm::vec3 &c, float radius, float width, const glm::vec4 &color, const glm::mat4 &projection, const glm::mat4 &view) const;
private:
	void InitGLEW();
	bool isInit = false;
	TexturesController *texturesCtrl;
	mutable Line line;
	mutable Circle circle;
};