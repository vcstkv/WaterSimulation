// calculate density and pressure
#version 460
#pragma optimize(off)

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle {
    vec2 r;
    vec2 v;
    vec2 f;
    vec2 prev_f;
    float p; // pressure
    float d; // density
    float m; // mass
    uint bucket;
};

struct Index{
	uint begin;
	uint end;
	uint n[9];
};

const float PI = 3.1415926535897932384626433832795;

uniform float h = 0.01;
uniform float k = 0.01;
uniform float rd = 1000.0;

layout(std430, binding = 0) buffer Particles
{
    Particle particles[];
};

layout(std430, binding = 4) buffer IndexTable
{
    Index indexTable[];
};

uniform uint index_max_neighbors = 40;

void main()
{
    uint p_i = gl_GlobalInvocationID.x;
	if (p_i >= particles.length())
	{
		return;
	}
	
    Particle p = particles[p_i];
	Index p_i_bucket = indexTable[p.bucket];
	
    const float k_poly6_coeff = 315.0/(64.0 * PI * pow(h, 9));
    const float h2 = h * h;

    p.d = rd;
	uint i = 0;
	uint j = 0;
	Index neighborIdx;
	Particle nighborP;
    for (i = 0; i < 9; i++)
	{	
		if (p_i_bucket.n[i] == 0xffffffff)
		{
			break;
		}
		neighborIdx = indexTable[p_i_bucket.n[i]];
		for(j = neighborIdx.begin; j <= neighborIdx.end; j++)
		{
			nighborP = particles[j];
	
			vec2 dr = p.r - nighborP.r;
			
			if (length(dr) > h)
			{
				continue;
			}
			
			float d_h2_dr2 = h2 - dot(dr, dr);
	
			p.d += nighborP.m * k_poly6_coeff * d_h2_dr2 * d_h2_dr2 * d_h2_dr2;
		}
    }

    p.p = k * (p.d - rd);
    particles[p_i] = p;
}
