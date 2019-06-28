#version 460

in vec2 p_location;

void main() {
    gl_Position = vec4(p_location, 0.0, 1.0);
}
