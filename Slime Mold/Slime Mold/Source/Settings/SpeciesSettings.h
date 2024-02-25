#pragma once

#include <array>

#include "Settings/SpawnModes.h"

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

inline std::array<const char*, 8> ColonyPresets = {
	"3,0.2|1,1,1,20,2,1,35,30,5,250000,2",
	"5,0.75|0,1,0.31,30,-3,1,20,112,2,250000,1",
	"3,0.5|1,0.06,0.16,50,30,1,20,45,8,12500,2|"
		"0.47,0.83,0.11,50,-90,1,30,112,8,12500,2",
	"3,0.5|0.54,0.06,1,70,30,1,20,60,12,8500,2|"
		"0.11,0.62,0.83,50,-90,1,30,112,12,8500,2|"
		"0.47,0.83,0.11,50,50,1,30,70,12,8500,2",
	"1,1|0.73,0.47,0.28,20,2,1,10,2,10,300000,0|"
		"0.08,0.46,0.29,20,2,1,20,2,10,300000,0",
	"4.6,0.1|0.86,0.58,0.03,30,11.23,1,-20,-80,0.8,80000,1",
	"6.89,0.06|0,0.81,0.86,18,3.28,1,5.58,81,2.9,80000,5|"
		"0.04,0,0.08,50,150,1,-18,92.5,1.5,200000,0",
	"17.82,0.86|1,0.27,0.6,60,50,1,11.2,100,2.33,300000,0|"
		"0.02,0.25,0,-10,-3,1,6.14,-14,7.2,80000,0"
};
