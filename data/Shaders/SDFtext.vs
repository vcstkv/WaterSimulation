#version 330 core

layout(location = 0) in vec3 vertexCoords;
layout(location = 1) in vec2 textureCoords;

uniform mat4 MVP;

out vec2 uv;

void main()
{   
    gl_Position = MVP * vec4(vertexCoords, 1.0);
	uv = textureCoords;
}