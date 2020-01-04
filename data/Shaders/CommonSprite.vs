#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec2 textureCoords;

out vec2 uv;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexCoords, 1.0);
	uv = textureCoords;
}