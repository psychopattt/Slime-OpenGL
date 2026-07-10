#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(std430) restrict readonly buffer trailBuffer {
    vec4 Trail[];
};

layout(std430) restrict writeonly buffer diffusedTrailBuffer {
    vec4 DiffusedTrail[];
};

uniform ivec2 size;
uniform float decayRate;
uniform float diffuseRate;
const float globalSpeed = 0.02;

vec4 GetAverageNeighborTrail(ivec2 position)
{
    vec4 totalTrail = vec4(0);

    for (int offsetY = -1; offsetY < 2; offsetY++)
    {
        for (int offsetX = -1; offsetX < 2; offsetX++)
        {
            uint id = clamp(position.y + offsetY, 0, size.y - 1) *
                size.x + clamp(position.x + offsetX, 0, size.x - 1);

            totalTrail += Trail[id];
        }
    }

    return totalTrail / 9;
}

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= size.x || position.y >= size.y)
        return;

    uint id = position.y * size.x + position.x;
    vec4 averageNeighborTrail = GetAverageNeighborTrail(position);
    float diffuseWeight = clamp(diffuseRate * globalSpeed, 0.0, 1.0);
    vec4 mergedTrail = (1 - diffuseWeight) * Trail[id] + diffuseWeight * averageNeighborTrail;
    vec4 decayedTrail = max(vec4(0), mergedTrail - decayRate * globalSpeed);

    DiffusedTrail[id] = decayedTrail;
}
