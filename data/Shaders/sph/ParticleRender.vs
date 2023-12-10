#version 460

layout(location = 0) in dvec2 r;
layout(location = 1) in dvec2 v;
layout(location = 2) in dvec2 f;
layout(location = 3) in dvec2 prev_f;
layout(location = 4) in double p; // pressure
layout(location = 5) in double d; // density
layout(location = 6) in double m; // mass

out vec4 pColor;

void main() {
    gl_Position = vec4(r, 0.0, 1.0);
    //vec3 c = normalize(vec3(p, d, m));
    double col = clamp((p + 500.)/1000., 0., 1.);
    vec3 c = vec3(col, 0., 1.-col);
    pColor = vec4(c, 1.);
}
