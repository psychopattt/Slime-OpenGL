#include "ColonyBuilder.h"

#include "Settings/Colony/SlimeCell.h"
#include "Settings/Colony/ColonySettings.h"
#include "Settings/SlimeMoldSettings.h"

using std::vector;

ColonyBuilder::ColonyBuilder()
{
	std::random_device randomDevice;
	randomEngine = std::mt19937(randomDevice());
	randomDistribution = std::uniform_real_distribution<float>(0, 1);
}

void ColonyBuilder::GenerateSettings(unsigned int width, unsigned int height)
{
	center[0] = width / 2.0f;
	center[1] = height / 2.0f;
	missileAngle = Random01() * SlimeMoldSettings::Tau;
	smallestDimension = width < height ? width : height;

	for (int i = 0; i < std::size(randomPoints); i++)
	{
		randomPoints[i][0] = Random01() * width;
		randomPoints[i][1] = Random01() * height;
	}
}

vector<SlimeCell> ColonyBuilder::BuildColony(unsigned int width,
	unsigned int height, int totalCells)
{
	GenerateSettings(width, height);

	int speciesIndex = 0;
	vector<SlimeCell> cells;
	cells.reserve(totalCells);

	for (SpeciesSettings& species : Colony)
	{
		species.wasEnabled = species.enabled;

		if (species.enabled)
		{
			BuildSpecies(
				cells, speciesIndex++,
				species, width, height
			);
		}
	}

	return cells;
}

void ColonyBuilder::BuildSpecies(vector<SlimeCell>& cells, int speciesIndex,
	SpeciesSettings& species, unsigned int width, unsigned int height)
{
	for (int i = 0; i < std::size(species.mask); i++)
		species.mask[i] = speciesIndex == i;

	for (int i = 0; i < species.cellCount; i++)
	{
		SlimeCell cell = SlimeCell();
		cell.speciesIndex = speciesIndex;

		GenerateCellSpawn(cell, species.spawnMode, width, height);
		cells.push_back(cell);
	}
}

void ColonyBuilder::GenerateCellSpawn(SlimeCell& cell, SpawnMode spawnMode,
	unsigned int width, unsigned int height)
{
	cell.angle = Random01() * SlimeMoldSettings::Tau;

	switch (spawnMode)
	{
		case Random:
			cell.position[0] = Random01() * width;
			cell.position[1] = Random01() * height;
			break;
		case Center:
			cell.position[0] = center[0];
			cell.position[1] = center[1];
			break;
		case Circle:
			RandomCirclePosition(cell.position, center, smallestDimension * 0.16f);
			break;
		case InwardCircle:
			RandomCirclePosition(cell.position, center, smallestDimension / 2.0f);
			cell.angle = atan2(center[1] - cell.position[1], center[0] - cell.position[0]);
			break;
		case OutwardCircle:
			RandomCirclePosition(cell.position, center, smallestDimension * 0.16f);
			cell.angle = atan2(cell.position[1] - center[1], cell.position[0] - center[0]);
			break;
		case RandomCircle:
			RandomCirclePosition(cell.position, randomPoints[cell.speciesIndex],
				smallestDimension * 0.16f);
			break;
		case RandomPoint:
			cell.position[0] = randomPoints[cell.speciesIndex][0];
			cell.position[1] = randomPoints[cell.speciesIndex][1];
			break;
		case Missile:
			cell.position[0] = center[0];
			cell.position[1] = center[1];
			cell.angle = missileAngle;
			break;
	}
}

void ColonyBuilder::RandomCirclePosition(float* position, float* center, float radius)
{
	float distanceFromCenter = radius * sqrt(Random01());
	float angle = Random01() * SlimeMoldSettings::Tau;

	position[0] = center[0] + distanceFromCenter * cos(angle);
	position[1] = center[1] + distanceFromCenter * sin(angle);
}

float ColonyBuilder::Random01()
{
	return randomDistribution(randomEngine);
}
