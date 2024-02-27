#version 460 core

layout(local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

struct SlimeCell
{
    vec2 position;
    float angle;
    int speciesIndex;
};

struct SpeciesSettings
{
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

layout(std430) restrict readonly buffer colonySettings
{
    SpeciesSettings Colony[];
};

uniform uint cellCount;
layout(std430) restrict buffer slimeCells
{
    SlimeCell Cells[];
};

uniform int width;
uniform int height;
layout(RGBA32F) restrict uniform image2D trailTexture;

uniform uint seed;
uniform uint userSeed;
const float globalSpeed = 0.02;
const float tau = 6.2831853072;

uint Random(uint state)
{
    state ^= 2747636419;
    state *= 2654435769;
    state ^= state >> 16;
    state *= userSeed + 1;
    state *= 2654435769;
    state ^= state >> 16;
    state *= 2654435769;
    return state;
}

float NormalizeUnsignedInt(uint value)
{
    return value / 4294967295.0;
}

float SenseTrail(SlimeCell cell, float sensorAngleOffset)
{
    SpeciesSettings species = Colony[cell.speciesIndex];

    int sensorSize = species.sensorSize;
    float sensorAngle = cell.angle + sensorAngleOffset;
    vec2 sensorDirection = vec2(cos(sensorAngle), sin(sensorAngle));
    vec2 sensorPosition = sensorDirection * species.sensorOffset + cell.position;

    ivec4 senseWeight = ivec4(species.mask * 2 - 1);
    float senseTotal = 0;

    for (int offsetY = -sensorSize; offsetY <= sensorSize; offsetY++)
    {
        for (int offsetX = -sensorSize; offsetX <= sensorSize; offsetX++)
        {
            ivec2 samplePosition = ivec2(
                clamp(int(sensorPosition.x) + offsetX, 0, width - 1),
                clamp(int(sensorPosition.y) + offsetY, 0, height - 1)
            );

            vec4 trailData = imageLoad(trailTexture, samplePosition);
            senseTotal += dot(senseWeight, trailData);
        }
    }

    return senseTotal;
}

void SteerCell(uint cellId, uint random)
{
    SlimeCell cell = Cells[cellId];
    SpeciesSettings species = Colony[cell.speciesIndex];

    float steerStrength = NormalizeUnsignedInt(random);
    float turnSpeed = species.turnSpeed * tau * globalSpeed;

    float forwardWeight = SenseTrail(cell, 0);
    float sensorAngleRad = radians(species.sensorAngleDegrees);
    float rightWeight = SenseTrail(cell, -sensorAngleRad);
    float leftWeight = SenseTrail(cell, sensorAngleRad);

    if (forwardWeight <= leftWeight || forwardWeight <= rightWeight)
    {
        if (forwardWeight < leftWeight && forwardWeight < rightWeight) // Turn randomly
            Cells[cellId].angle += (steerStrength - 0.5) * 2 * turnSpeed;
        else if (rightWeight > leftWeight) // Turn right
            Cells[cellId].angle -= steerStrength * turnSpeed;
        else if (leftWeight > rightWeight) // Turn left
            Cells[cellId].angle += steerStrength * turnSpeed;
    }
}

void MoveCell(uint cellId, uint random)
{
    SlimeCell cell = Cells[cellId];
    SpeciesSettings species = Colony[cell.speciesIndex];

    vec2 direction = vec2(cos(cell.angle), sin(cell.angle));
    vec2 newPos = cell.position + direction * species.moveSpeed * globalSpeed;

    // Bounce on boundaries
    if (newPos.x < 0 || newPos.x >= width || newPos.y < 0 || newPos.y >= height)
    {
        float randomAngle = NormalizeUnsignedInt(random) * tau;
        Cells[cellId].angle = randomAngle;

        newPos.x = clamp(newPos.x, 0, width - 1);
        newPos.y = clamp(newPos.y, 0, height - 1);
    }
    else
    {
        ivec2 newTrailCoords = ivec2(newPos);
        vec4 oldTrailData = imageLoad(trailTexture, newTrailCoords);
        vec4 newTrailData = min(vec4(1),
            oldTrailData + species.mask * species.trailWeight * globalSpeed
        );

        imageStore(trailTexture, newTrailCoords, newTrailData);
    }

    Cells[cellId].position = newPos;
}

void main()
{
    uint cellId = gl_GlobalInvocationID.x;

    if (cellId >= cellCount)
        return;

    vec2 position = Cells[cellId].position;
    uint positionId = uint(position.y * width + position.x);
    uint random = Random(positionId + Random(seed * 100000 + cellId));

    SteerCell(cellId, random);
    MoveCell(cellId, Random(random));
}
