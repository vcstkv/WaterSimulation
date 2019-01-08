#version 330 core

layout(location = 0) in vec2 vertexCoords;
layout(location = 1) in vec2 textureCoords;

uniform mat4 MVP;
uniform vec2 cords[4];

out vec2 uv;

void main()
{   
    uv = vertexCoords*cords[3]+cords[2];
    gl_Position = MVP * vec4(vertexCoords*cords[1]+cords[0], 0.0, 1.0);
}