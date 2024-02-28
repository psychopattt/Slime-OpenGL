#pragma once

enum SpawnMode : int
{
	Random = 0,
	Center = 1,
	Circle = 2,
	InwardCircle = 3,
	OutwardCircle = 4,
	RandomCircle = 5,
	RandomPoint = 6,
	Missile = 7
};

constexpr char SpawnModeLabels[] = "Random\0Center\0Circle\0"
	"Inward Circle\0Outward Circle\0Random Circle\0"
	"Random Point\0Missile\0\0";
