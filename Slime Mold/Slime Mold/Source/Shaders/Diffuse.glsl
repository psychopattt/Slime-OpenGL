#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform int width;
uniform int height;
uniform float decayRate;
uniform float diffuseRate;
restrict writeonly uniform image2D diffusedTrailTexture;
layout(RGBA32F) restrict readonly uniform image2D trailTexture;

const float globalSpeed = 0.02;

vec4 GetAverageNeighborTrail(ivec2 position)
{
    vec4 totalTrail = vec4(0);

    for (int offsetY = -1; offsetY < 2; offsetY++)
    {
        for (int offsetX = -1; offsetX < 2; offsetX++)
        {
            ivec2 samplePosition = ivec2(
                clamp(position.x + offsetX, 0, width - 1),
                clamp(position.y + offsetY, 0, height - 1)
            );

            totalTrail += imageLoad(trailTexture, samplePosition);
        }
    }

    return totalTrail / 9;
}

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= width || position.y >= height)
        return;

    vec4 trail = imageLoad(trailTexture, position);
    vec4 averageNeighborTrail = GetAverageNeighborTrail(position);
    float diffuseWeight = clamp(diffuseRate * globalSpeed, 0.0, 1.0);
    vec4 mergedTrail = (1 - diffuseWeight) * trail + diffuseWeight * averageNeighborTrail;
    vec4 decayedTrail = max(vec4(0), mergedTrail - decayRate * globalSpeed);

    imageStore(diffusedTrailTexture, position, decayedTrail);
}
