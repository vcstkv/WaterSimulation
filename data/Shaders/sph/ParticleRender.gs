#version 460

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 mvp;
uniform mat4 p;
uniform float k_ring_radius;
uniform float k_ring_width;
uniform float p_ring_radius;
uniform float p_ring_width;

in vec4 pColor[];

out vec4 color;
out vec2 uv;
out float pri;
out float pro;
out float kri;

void main() {
	pri = (p_ring_radius - p_ring_width) / k_ring_radius * 0.5;
	pro = p_ring_radius / k_ring_radius * 0.5;
	kri = (k_ring_radius - k_ring_width) / k_ring_radius * 0.5;
    gl_Position = mvp * (gl_in[0].gl_Position + vec4(k_ring_radius, k_ring_radius, 0.0, 0.0));
    color = pColor[0];
	uv = vec2(1.0, 1.0);
    EmitVertex();
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(-k_ring_radius, k_ring_radius, 0.0, 0.0));
	color = pColor[0];
	uv = vec2(0.0, 1.0);
    EmitVertex();
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(k_ring_radius, -k_ring_radius, 0.0, 0.0));
	color = pColor[0];
	uv = vec2(1.0, 0.0);
    EmitVertex();
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(-k_ring_radius, -k_ring_radius, 0.0, 0.0));
	color = pColor[0];
	uv = vec2(0.0, 0.0);
    EmitVertex();
    EndPrimitive();

}
