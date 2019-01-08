#define GRAPHICS_EXPORTS
#include "Graphics/Shapes/Sprites/CommonSprite.h"



CommonSprite::CommonSprite(float x, float y) 
	: Sprite(x, y)
{
	currentShaderProgram = -1;
}

std::vector<SpriteShaderProgram*> CommonSprite::shaderPrograms;
void CommonSprite::SetShader(const char *vFilePath, const char *fFilePath)
{
	/*bool isMatch = false;
	for (unsigned int i = 0; i < shaderPrograms.size(); i++)
	{
		if (shaderPrograms[i]->IsShaderEqual(vFilePath, fFilePath))
		{
			isMatch = true;
			currentShaderProgram = i;
		}
	}
	if (!isMatch)
	{*/
		shaderPrograms.push_back(new SpriteShaderProgram(vFilePath, fFilePath));
		currentShaderProgram = shaderPrograms.size() - 1;
	//}
}

void CommonSprite::Draw(glm::mat4 *projection, glm::mat4 *view)
{
	if (shaderPrograms.empty())
	{
		std::cout << "There are no any shaders attached to sprite";
		return;
	}

	if (currentShaderProgram == -1)
	{
		std::cout << "Shader is not set";
		return;
	}

	//std::cout << std::endl << "ShaderInfo: " << currentShaderProgram;
	shaderPrograms[currentShaderProgram]->Enable();
	//std::cout << std::endl << shaderPrograms[currentShaderProgram]->id;

	if (textureId > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(shaderPrograms[currentShaderProgram]->texSamplerShLoc, 0);
		//std::cout << std::endl << shaderPrograms[currentShaderProgram]->texSamplerShLoc;
	}

	glUniformMatrix4fv(shaderPrograms[currentShaderProgram]->mvpShLoc, 1, GL_FALSE, &((*projection) * (*view) * model)[0][0]);
	//std::cout << std::endl << shaderPrograms[currentShaderProgram]->mvpShLoc;

	glUniform4fv(shaderPrograms[currentShaderProgram]->matDiffColorShLoc, 1, &(color)[0]);
	//std::cout << std::endl << shaderPrograms[currentShaderProgram]->matDiffColorShLoc;

	if (textureId > 0 && color.x >= 0)
	{
		glUniform1i(shaderPrograms[currentShaderProgram]->isColoredShLoc, 1);
	}
	else
	{
		glUniform1i(shaderPrograms[currentShaderProgram]->isColoredShLoc, 0);
	}
	//std::cout << std::endl << shaderPrograms[currentShaderProgram]->isColoredShLoc;

	glBindVertexArray(vertexArrayObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	shaderPrograms[currentShaderProgram]->Disable();
}
