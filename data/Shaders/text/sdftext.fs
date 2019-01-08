#version 330 core

in vec2 uv;

// Ouput data
out vec4 resultColor;

uniform sampler2D textureSampler;
uniform vec4 color;
uniform vec2 params;



void main(void)
{
    float tx = texture2D(textureSampler, uv).r;
    float a = min((tx-params.x)*params.y, 1.0);
    resultColor = vec4(color.rgb,a*color.a);
}