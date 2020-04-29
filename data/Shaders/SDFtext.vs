#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec2 textureCoords;

uniform mat4 mvp;
uniform mat3 textureRegion;

out vec2 uv;

void main()
{   
    gl_Position = mvp * vec4(vertexCoords, 1.0);
	uv = (textureRegion * vec3(textureCoords.x, 1.0 - textureCoords.y, 1.0)).xy;
}