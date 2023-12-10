// leapfrog integration of movement equations
#version 460
//#pragma optimize(off)

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle {
    dvec2 r;
    dvec2 v;
    dvec2 f;
    dvec2 prev_f;
    double p; // pressure
    double d; // density
    double m; // mass
    uint bucket;
    uint pad;
};

uniform double dt = 0.01;

layout(std430, binding = 0) buffer Particles {
    Particle current_particles[];
};

void main()
{
    uint gid = gl_GlobalInvocationID.x;	
	if (gid >= current_particles.length())
	{
		return;
	}
	
    Particle p = current_particles[gid];

    dvec2 v_half = p.v + 0.5 * dt * p.prev_f / p.d;
    p.r = p.r + v_half * dt;
    p.v = v_half + 0.5 * dt * p.f / p.d;
    p.prev_f = p.f; // save from previous step

    current_particles[gid] = p;
}
