#pragma once

enum SpawnMode : int
{
	Random = 0,
	Point = 1,
	InwardCircle = 2,
	OutwardCircle = 3,
	RandomCircle = 4,
	RandomPoints = 5,
	Missile = 6
};

constexpr char SpawnModeLabels[] = "Random\0Point\0Inward Circle"
	"\0Outward Circle\0Random Circle\0Random Points\0Missile\0\0";
