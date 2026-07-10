#version 460 core

layout(local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

struct SlimeCell {
    vec2 position;
    float angle;
    int speciesIndex;
};

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

layout(std430) restrict buffer slimeCells {
    SlimeCell Cells[];
};

layout(std430) restrict buffer trailBuffer {
    vec4 Trail[];
};

uniform ivec2 size;
uniform uint seed;
uniform uint userSeed;
uniform uint cellCount;
uniform float globalSpeed;
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
            uint id = clamp(int(sensorPosition.y) + offsetY, 0, size.y - 1) *
                size.x + clamp(int(sensorPosition.x) + offsetX, 0, size.x - 1);

            senseTotal += dot(senseWeight, Trail[id]);
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
    if (newPos.x < 0 || newPos.x >= size.x || newPos.y < 0 || newPos.y >= size.y)
    {
        newPos = clamp(newPos, vec2(0), size - 1);
        Cells[cellId].angle = NormalizeUnsignedInt(random) * tau;
    }
    else
    {
        uint newTrailId = int(newPos.y) * size.x + int(newPos.x);
        vec4 newTrailData = species.mask * species.trailWeight *
            globalSpeed + Trail[newTrailId];

        Trail[newTrailId] = min(vec4(1), newTrailData);
    }

    Cells[cellId].position = newPos;
}

void main()
{
    uint cellId = gl_GlobalInvocationID.x;

    if (cellId >= cellCount)
        return;

    vec2 position = Cells[cellId].position;
    uint positionId = uint(position.y * size.x + position.x);
    uint random = Random(positionId + Random(seed * 100000 + cellId));

    SteerCell(cellId, random);
    MoveCell(cellId, Random(random));
}
