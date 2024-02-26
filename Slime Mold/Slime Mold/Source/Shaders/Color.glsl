#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

struct SpeciesSettings
{
    vec4 mask;
    vec4 color;

    float moveSpeed;
    float turnSpeed;

    int sensorSize;
    float sensorOffset;
    float sensorAngleDegrees;

    float trailWeight;
};

uniform uint colonySize;
layout(std430) restrict readonly buffer colonySettings
{
    SpeciesSettings Colony[];
};

layout(RGBA32F) restrict readonly uniform image2D trailTexture;
restrict writeonly uniform image2D displayTexture;
uniform int height;
uniform int width;

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= width || position.y >= height)
        return;

    vec4 trailData = imageLoad(trailTexture, position);
    vec4 color = vec4(0);

    for (uint i = 0; i < colonySize; i++)
        color += Colony[i].color * dot(trailData, Colony[i].mask);

    imageStore(displayTexture, position, color);
}
