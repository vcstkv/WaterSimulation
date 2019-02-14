#define GRAPHICS_EXPORTS
#include "Graphics/Graphics.h"

Graphics::Graphics()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW init error");
		return;
	}

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

	texturesController = new TexturesController();
	Sprite::SetTexturesController(texturesController);
	line = new Line();
	circle = new Circle();
	//arialFont = new TextFont("..//..//Data//Fonts//arial.fnt");
	//chillerFont = new TextFont("..//..//Data//Fonts//Chiller.fnt");
	//calibriFont = new TextFont("..//..//Data//Fonts//calibri.fnt");
	//sagoePrintFont = new TextFont("..//..//Data//Fonts//sagoePrint.fnt");
}

Graphics::~Graphics()
{
	delete line;
	delete circle;
	//delete arialFont;
	//delete chillerFont;
	//delete calibriFont;
	//delete sagoePrintFont;
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, float width, glm::vec4 *color, glm::mat4 *projection)
{
	line->SetLine(x1, y1, x2, y2, width, color);
	line->Draw(projection);
}

void Graphics::DrawCircle(float x, float y, float radius, float width, glm::vec4 *color, glm::mat4 *projection, glm::mat4 *view)
{
	circle->SetCircle(x, y, radius, width, color);
	circle->Draw(projection, view);
}

void Graphics::SetScreenSize(int width, int height)
{
	screenHeight = height;
	screenWidth = width;
}

//void Graphics::DrawText(TextBlock *textBlock, glm::mat4 *projection, SpriteShaderProgram *spriteShader, bool isRected)
//{
//	textBlock->font->DrawText(textBlock->text, textBlock->fontSize, &textBlock->color, textBlock->x, textBlock->y, projection, spriteShader, isRected);
//}



