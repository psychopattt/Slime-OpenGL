#pragma once

#include <array>

#include "Settings/SpawnMode.h"

struct ShaderSpeciesSettings
{
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
			{ color[0], color[1], color[2], color[3] },
			moveSpeed, turnSpeed, sensorSize, sensorOffset,
			sensorAngleDegrees, trailWeight
		);
	}
};

inline std::array<SpeciesSettings, 3> Colony = {
	SpeciesSettings({ 1, 1, 1, 0 }, 20, 2, 1, 35, 30, 10, 100, Point, true),
	SpeciesSettings({ 1, 0, 0, 0 }, 20, 2, 1, 35, 30, 10, 100, Point, false),
	SpeciesSettings({ 0, 1, 0, 0 }, 20, 2, 1, 35, 30, 10, 100, Point, false)
};
