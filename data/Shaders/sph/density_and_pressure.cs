// calculate density and pressure
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

struct Index{
	uint begin;
	uint end;
	uint n[9];
};

const double PI = 3.1415926535897932384626433832795;

uniform double h = 0.01;
uniform double k = 0.01;
uniform double rd = 1000.0;

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

	const double h2 = h * h;
    const double k_poly6_coeff = 315.0/(64.0 * PI * h2 * h2 * h2 * h2 * h);

    p.d = 0;
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
	
			dvec2 dr = p.r - nighborP.r;
			
			double d_h2_dr2 = h2 - dot(dr, dr);
			
			if (d_h2_dr2 < 0.)
			{
				continue;
			}
	
			p.d += nighborP.m * k_poly6_coeff * d_h2_dr2 * d_h2_dr2 * d_h2_dr2;
		}
    }

	if (p.d <= 0) {
		p.d = 0.00001;
	}		
    p.p = k * (p.d - rd);
    particles[p_i] = p;
}
