// create index
#version 460

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Index{
	uint begin;
	uint end;
	uint n[9];
};

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

layout(std430, binding=0) buffer Particles {
    Particle particles[];
};

layout(std430, binding=4) buffer IndexTable
{
    Index indexTable[];
};

uniform uint particlesCount;

void main()
{
    uint i = gl_GlobalInvocationID.x;
	if(i >= particlesCount)
	{
		return;
	}
	Particle p = particles[i];
	atomicMin(indexTable[p.bucket].begin, i);
	atomicMax(indexTable[p.bucket].end, i);
	//barrier();
	memoryBarrier();
}