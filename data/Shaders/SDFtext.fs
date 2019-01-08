#version 330 core

in vec2 uv;

out vec4 resultColor;

uniform sampler2D tex;
uniform vec4 params;
uniform vec4 color;
uniform vec4 borderColor;

void main(void)
{
	//float tx = texture2D(tex, uv).r;
    //float a = min((tx - params.x) * params.y, 1.0);
    //resultColor = vec4(color.rgb, a);

	float tx = texture2D(tex, uv).r;
    float b = min((tx-params.z)*params.w, 1.0);
    float a = clamp((tx-params.x)*params.y, 0.0, 1.0);
    resultColor = vec4(borderColor.rgb+(color.rgb-borderColor.rgb)*a, b);
}