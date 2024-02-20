#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(RGBA32F) restrict readonly uniform image2D source;
layout(RGBA32F) restrict writeonly uniform image2D target;

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);
    imageStore(target, position, imageLoad(source, position));
}
