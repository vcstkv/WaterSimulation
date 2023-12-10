// calculate forces
#version 460

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

uniform double g = 0.08; // gravity
uniform double mu = 5.0; // viscosity coefficient
uniform double h = 0.01; // smoothing parameter
uniform double s_tens_coeff = 2.7;
uniform double s_tens_tresh = 0.001;
uniform uint index_max_neighbors = 40; // maximum number of neighbors in the index

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

layout(std430, binding = 4) buffer IndexTable
{
    Index indexTable[];
};

void main()
{
    uint p_i = gl_GlobalInvocationID.x;
	if (p_i >= particles.length())
	{
		return;
	}
	
    Particle p = particles[p_i];
    Index p_i_bucket = indexTable[p.bucket];

    const double k_h6 = h * h * h * h * h * h;
    const double k_grad_coeff = -45.0f / (PI * k_h6);
    const double k_lap_coeff = 45.f / (PI * k_h6);
	const double k_grad_poly6 = -945.f / (32.f * PI * k_h6 * h * h * h);

    dvec2 f_press = dvec2(.0f, .0f);
    dvec2 f_vis = dvec2(.0f, .0f);
	dvec2 f_s_tens = dvec2(.0f, .0f);
	dvec2 s_normal = dvec2(.0f, .0f);
	double rel_mass = 0.0f;
	double c = 0;
	
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
            double ldr = length(dr);
			
			if (ldr > h)
			{
				continue;
			}
			
            dvec2 ndr = ldr > 0 ? (dr/ldr) : dvec2(0, 0);
			rel_mass = nighborP.m / nighborP.d;
			
            // pressure force
            f_press += ndr * rel_mass * 0.5 * (nighborP.p + p.p) * k_grad_coeff * (h - ldr) * (h - ldr);

            // viscosity force
            f_vis += rel_mass * (nighborP.v - p.v) * k_lap_coeff * (h - ldr);
			
			// surface tension force
			dvec2 kernel = ndr * k_grad_poly6 * (h * h - ldr * ldr) * (h * h - ldr * ldr);
			f_s_tens += kernel * rel_mass;
			c += length(kernel) * rel_mass;
        }
    }

	if (length(f_s_tens) > s_tens_tresh)
	{
		f_s_tens = normalize(f_s_tens) * -s_tens_coeff * c;
	}
	else
	{
		f_s_tens = dvec2(.0f, .0f);
	}
	
    dvec2 f_grav = dvec2(0, g * p.d);

    p.f = -f_press + f_grav + mu * f_vis + f_s_tens;

    particles[p_i] = p;
}
