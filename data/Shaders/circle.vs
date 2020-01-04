#version 430 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec2 textureCoords;

uniform vec2 radius;
uniform mat4 mvp;

out vec2 uv;
out vec2 r;

void main()
{
	gl_Position =  mvp * vec4(vertexCoords, 1.0);
	uv = textureCoords;
}