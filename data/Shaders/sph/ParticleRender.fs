#version 460

in vec2 uv;
in float pri;
in float pro;
in float kri;
in vec4 color;
out vec4 frag_color;

void main() 
{
	vec2 c = uv - vec2(0.5);
	float dist = sqrt(dot(c, c));
    //if (dist >= 0.5 /*|| dist < pri*/ || (dist > pro && dist < kri))
	if (dist > pro)
    { 
		discard;
	}
    frag_color = color;
	/*if(dist > kri)
	{
		frag_color.b = 0.0;
		frag_color.g = 1.0;
	}*/
}
