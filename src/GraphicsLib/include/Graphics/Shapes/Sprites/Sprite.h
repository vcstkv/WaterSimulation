#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Textures/TexturesController.h"

class Sprite
{

public:
	GRAPHICS_API Sprite(float x, float y);
	GRAPHICS_API ~Sprite();
	GRAPHICS_API virtual void Draw(glm::mat4 *projection, glm::mat4 *view) = 0;
	GRAPHICS_API void SetSize(float width, float height);
	GRAPHICS_API void SetTextureShape(float uvX, float uvY, int width, int height);
	GRAPHICS_API void SetColor(glm::vec4 *color);
	GRAPHICS_API void SetTexture(const char* filePath, bool isStdFolder = true);
	GRAPHICS_API void Scale(glm::vec3 *delta);
	GRAPHICS_API void SetScale(glm::vec3 *scale);
	GRAPHICS_API void SetPosition(float x, float y);
	GRAPHICS_API void SetPosition(glm::vec3 *position);
	GRAPHICS_API void Translate(float dx, float dy);
	GRAPHICS_API void Translate(glm::vec3 *dPos);
	GRAPHICS_API virtual void SetShader(const char *vFilePath, const char *fFilePath) = 0;
	GRAPHICS_API void SetActiveShaderProgram(int localShaderProgramId);
	static void SetTexturesController(TexturesController *texturesController);
	float width;
	float height;
protected:
	glm::mat4 model;
	void InitializeSprite();
	static const int8_t vertexCount = 4;
	static const int8_t indexCount = 6;
	glm::vec4 color;
	static TexturesController *texturesController;
	GLuint textureId;
	int currentShaderProgram;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
};

