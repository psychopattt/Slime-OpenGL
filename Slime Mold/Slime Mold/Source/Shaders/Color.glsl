#version 460 core

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

struct SpeciesSettings {
    vec4 mask;
    vec4 mainColor;
    vec4 edgeColor;

    float moveSpeed;
    float turnSpeed;

    int sensorSize;
    float sensorOffset;
    float sensorAngleDegrees;

    float trailWeight;
};

layout(std430) restrict readonly buffer colonySettings {
    SpeciesSettings Colony[];
};

layout(std430) restrict readonly buffer trailBuffer {
    vec4 Trail[];
};

restrict writeonly uniform image2D displayTexture;

uniform ivec2 size;
uniform uint colonySize;

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= size.x || position.y >= size.y)
        return;

    vec4 trailData = Trail[position.y * size.x + position.x];
    vec4 color = vec4(0);

    for (uint i = 0; i < colonySize; i++)
    {
        SpeciesSettings species = Colony[i];
        float trailIntensity = dot(trailData, species.mask);
        vec4 mixedColor = mix(species.edgeColor, species.mainColor, trailIntensity);
        color += mixedColor * trailIntensity;
    }

    imageStore(displayTexture, position, color);
}
