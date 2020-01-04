#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Textures/TexturesController.h"

class Sprite
{

public:
	GRAPHICS_API Sprite(const glm::vec3 &c);
	GRAPHICS_API ~Sprite();
	GRAPHICS_API virtual void Draw(const glm::mat4 &projection, const glm::mat4 &view) = 0;
	GRAPHICS_API void SetSize(float width, float height);
	GRAPHICS_API void SetTextureShape(float uvX, float uvY, int width, int height);
	GRAPHICS_API void SetColor(const glm::vec4 &color);
	GRAPHICS_API void SetTexture(const char* filePath, bool isStdFolder = true);
	GRAPHICS_API void Scale(const glm::vec3 &delta);
	GRAPHICS_API void SetScale(const glm::vec3 &scale);
	GRAPHICS_API void SetPosition(float x, float y, float z);
	GRAPHICS_API void SetPosition(const glm::vec3 &position);
	GRAPHICS_API void Translate(float dx, float dy, float dz);
	GRAPHICS_API void Translate(const glm::vec3 &dPos);
	GRAPHICS_API virtual void SetShader(const char *vFilePath, const char *fFilePath) = 0;
	GRAPHICS_API void SetActiveShaderProgram(int localShaderProgramId);
	static void SetTexturesController(TexturesController *texturesController);
	float width = 1.f;
	float height = 1.f;
protected:
	glm::mat4 model{1.f};
	void InitializeSprite();
	static const int8_t vertexCount = 4;
	static const int8_t indexCount = 6;
	static const int8_t dim = 3;
	glm::vec4 color{1.f};
	static TexturesController *texturesController;
	GLuint textureId = 0;
	int currentShaderProgram = -1;
	GLuint vertexBuffer = 0;
	GLuint textureBuffer = 0;
	GLuint indexBuffer = 0;
	GLuint vertexArrayObject = 0;
};

