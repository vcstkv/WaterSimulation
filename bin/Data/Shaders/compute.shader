#version 460

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable


uniform int numVertices;

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout( std140, binding=1 ) buffer Pos {
    vec3 pos[];
};

void main() {

    uint i = gl_GlobalInvocationID.x;

    // thread block size may not be exact multiple of number of particles
    if (i >= numVertices) return;

    pos[i] += 0.001;
}