#pragma once
#include "Graphics/GLHeaders.h"
#include <string>

using namespace glm;
class Model
{
public:
	GRAPHICS_API Model(std::string filePath, GLuint shadingProgramId);
	GRAPHICS_API Model();
	GRAPHICS_API ~Model();
	GRAPHICS_API void Draw(glm::mat4 *model, glm::mat4 *projection, glm::mat4 *view, glm::vec3 *lightPos);
	GRAPHICS_API void AddTexture(GLuint textureId);
	GRAPHICS_API int GetVerticesCount() { return vCoordCount/3; }
	GRAPHICS_API GLuint GetVerticesBuffer() { return vertexShaderBuffer; }
	vec3 diffColor = vec3( 1.0, 1.0, 1.0 );
	vec3 ambColor = { 0.2, 0.2, 0.2 };
	vec3 specColor = { 0, 0, 0 };
	vec3 lightColor = { 1.0, 1.0, 1.0};
	float lightPower = 5;
	

private:
	GLuint vertexBuffer;
	GLuint vertexShaderBuffer;
	GLuint indexBuffer;
	GLuint normalBuffer;
	GLuint textureBuffer;
	GLuint textureId;
	GLuint alphaId;
	int vCoordCount;
	int nCoordCount;
	int tCoordCount;
	int indexCount;

	GLuint shadingProgramID;
	GLint mvpShLoc;
	GLint mShLoc;
	GLint vShLoc;

	GLint lightPosShLoc;
	GLint lightColorShLoc;
	GLint lightPowerShLoc;

	GLint matDiffColorShLoc;
	GLint matSpecColorShLoc;
	GLint matAmbColorShLoc;
	GLint texSamplerShLoc;
	GLint alphaSamplerShLoc;

	void LoadModel(std::string filePath);
	void AddShader(GLuint shadingProgramId);
};

