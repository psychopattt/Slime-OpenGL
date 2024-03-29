#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f) restrict readonly uniform image2D source;
layout(rgba32f) restrict writeonly uniform image2D target;

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);
    imageStore(target, position, imageLoad(source, position));
}
