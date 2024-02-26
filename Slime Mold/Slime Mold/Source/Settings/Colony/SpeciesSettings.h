#pragma once

#include "Settings/Colony/SpawnModes.h"

struct ShaderSpeciesSettings
{
	float mask[4];
	float color[4];

	float moveSpeed;
	float turnSpeed;

	int sensorSize;
	float sensorOffset;
	float sensorAngleDegrees;

	float trailWeight;

	int padding[2];
};

struct SpeciesSettings
{
	float mask[4];
	float color[4];

	float moveSpeed;
	float turnSpeed;

	int sensorSize;
	float sensorOffset;
	float sensorAngleDegrees;

	float trailWeight;

	int cellCount;
	SpawnMode spawnMode;

	bool enabled;
	bool wasEnabled;

	ShaderSpeciesSettings ToShaderStruct() const
	{
		return ShaderSpeciesSettings(
			{ mask[0], mask[1], mask[2], mask[3] },
			{ color[0], color[1], color[2], color[3] },
			moveSpeed, turnSpeed, sensorSize, sensorOffset,
			sensorAngleDegrees, trailWeight
		);
	}
};
