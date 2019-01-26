#pragma once

#include "GLHeaders.h"
#include "Shaders/SpriteShaderProgram.h"
#include "Textures/TGALoader.h"
#include "Shapes/Line.h"
#include "Shapes/Circle.h"
//#include "TextBlock.h"
#include "Shapes/Sprites/Sprite.h"
#include "Textures/TexturesController.h"
#include <vector>
#include "GraphicsResources.h"

class Graphics
{
public:
	GRAPHICS_API Graphics();
	GRAPHICS_API ~Graphics();
	//GRAPHICS_API void DrawText(TextBlock *textBlock, glm::mat4 *projection, SpriteShaderProgram *spriteShader, bool isRected);
	GRAPHICS_API void DrawLine(float x1, float y1, float x2, float y2, glm::vec4 *color, glm::mat4 *projection);
	GRAPHICS_API void DrawCircle(float x, float y, float radius, float width, glm::vec4 *color, glm::mat4 *projection, glm::mat4 *view);
	GRAPHICS_API void SetScreenSize(int width, int height);
	int screenWidth;
	int screenHeight;

	/*TextFont *arialFont;
	TextFont *chillerFont;
	TextFont *calibriFont;
	TextFont *sagoePrintFont;*/
	//std::vector<TextBlock*> textBlocks;
private:
	TexturesController *texturesController;
	Line *line;
	Circle *circle;
};