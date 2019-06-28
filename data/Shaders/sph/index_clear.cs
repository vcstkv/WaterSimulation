// create index
#version 460

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Index{
	uint begin;
	uint end;
	uint n[9];
};

layout(std430, binding=4) buffer IndexTable
{
    Index indexTable[];
};

uniform uint particlesCount;
uniform uint indexTableSize;

void main()
{
    uint i = gl_GlobalInvocationID.x;
	if(i >= indexTableSize)
	{
		return;
	}
	Index index = indexTable[i];
	index.begin = particlesCount;
	index.end = 0;
	indexTable[i] = index;
}