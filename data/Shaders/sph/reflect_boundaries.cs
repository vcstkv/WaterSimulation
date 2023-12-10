// reflect boundaries
#version 460
#pragma optimize(off)

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

uniform double damping_coeff = 0.5;

layout(std430, binding=0) buffer Particles {
    Particle current_particles[];
};

uniform double boxMinX = 0.0;
uniform double boxMaxY = 1.5;
uniform double boxMinY = 0.0;
uniform double boxMaxX = 1.8;

void main()
{
    uint gid = gl_GlobalInvocationID.x;
	if (gid >= current_particles.length())
	{
		return;
	}
    Particle p = current_particles[gid];
	
	double eps = 0.00001 * (boxMaxX - boxMinX);

    if (p.r.y > boxMaxY)
	{
        //p.v.x *= damping_coeff;
		p.v.y *= -damping_coeff;
        p.r.y = boxMaxY /*- eps*/;
    }
	
	if (p.r.x > boxMaxX)
	{
        p.v.x *= -damping_coeff;
		//p.v.y *= damping_coeff;
        p.r.x = boxMaxX /*- eps*/;
    }
	
	if (p.r.y < boxMinY)
	{
        //p.v.x *= damping_coeff;
		p.v.y *= -damping_coeff;
        p.r.y = boxMinY /*+ eps*/;
    } 
	
	if (p.r.x < boxMinX)
	{
        p.v.x *= -damping_coeff;
		//p.v.y *= damping_coeff;
        p.r.x = boxMinX /*+ eps*/;
    }

    current_particles[gid] = p;
}
