#version 430 core

in vec2 uv;
uniform float innerRadius;
uniform vec4 color;
out vec4 resultColor;

void main()
{
	vec2 c = uv - vec2(0.5);
    float dist = sqrt(dot(c, c));
    if (dist >= 0.5 || dist < innerRadius)
    { 
		discard;
	}
    resultColor = color;
	/*resultColor.a = smoothstep(0.5, 0.5 - 0.01, dist) 
	                * smoothstep(innerRadius, innerRadius + 0.01, dist);*/
}