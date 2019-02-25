#version 460

//#pragma optimize(off)

in vec2 p_location;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(p_location.xy, 0, 1);
    gl_PointSize = 5.0;
}
