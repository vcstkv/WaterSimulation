#pragma once
#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport) 
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include "GLHeaders.h"
#include "Shaders/SpriteShaderProgram.h"
#include "Textures/TGALoader.h"
#include "Shapes/Line.h"
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
	void SetScreenSize(int width, int height) { screenHeight = height; screenWidth = width; }
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
};