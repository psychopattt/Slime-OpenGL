#pragma once

#include <array>

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
	bool enabled;
};

inline std::array<SpeciesSettings, 3> AllSpecies = {
	SpeciesSettings({ 1, 1, 1, 0 }, 20, 2, 1, 35, 30, 10, 100, true),
	SpeciesSettings({ 1, 0, 0, 0 }, 20, 2, 1, 35, 30, 10, 100, false),
	SpeciesSettings({ 0, 1, 0, 0 }, 20, 2, 1, 35, 30, 10, 100, false)
};

inline std::array<bool, 3> EnabledSpecies;
