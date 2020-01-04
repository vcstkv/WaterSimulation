#version 330 core

in vec2 uv;

// Ouput data
out vec4 resultColor;

uniform vec4 materialDiffuseColor;
uniform sampler2D textureSampler;
uniform int isColoredTexture;

void main()
{
	vec4 texColor = materialDiffuseColor;
	if (materialDiffuseColor.r >= 0)
	{
		if (isColoredTexture == 1)
		{
			texColor = materialDiffuseColor * texture(textureSampler, uv);
		}
	}
	else
	{
		texColor = texture(textureSampler, uv);
	}
	resultColor = texColor;
}