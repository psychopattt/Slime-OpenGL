#include "ColonyBuilder.h"

#include "Settings/Colony/SlimeCell.h"
#include "Settings/Colony/ColonySettings.h"
#include "Settings/SlimeMoldSettings.h"

using std::vector;
using randomInt = std::uniform_int_distribution<int>;
using randomFloat = std::uniform_real_distribution<float>;

ColonyBuilder::ColonyBuilder()
{
	std::random_device randomDevice;
	randomEngine = std::mt19937(randomDevice());
	randomPercent = randomFloat(0, 1);
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

void ColonyBuilder::RandomizeColony()
{
	for (int i = 0; i < Colony.size(); i++)
	{
		Colony[i].enabled = i == 0 || randomInt(0, 1)(randomEngine) == 0;

		if (!Colony[i].enabled)
			continue;

		for (int channelId = 0; channelId < 3; channelId++)
		{
			Colony[i].mainColor[channelId] = Random01();
			Colony[i].edgeColor[channelId] = Random01();
		}

		Colony[i].moveSpeed = randomFloat(1, 100)(randomEngine);
		Colony[i].turnSpeed = randomFloat(0.1f, 10)(randomEngine);
		Colony[i].sensorSize = randomInt(1, 3)(randomEngine);
		Colony[i].sensorOffset = randomFloat(-30, 100)(randomEngine);
		Colony[i].sensorAngleDegrees = randomFloat(1, 100)(randomEngine);
		Colony[i].trailWeight = randomFloat(1, 20)(randomEngine);
		Colony[i].cellCount = randomInt(1000, 1000000)(randomEngine);
		Colony[i].spawnMode = SpawnMode(randomInt(0, SpawnMode::Missile)(randomEngine));
	}

	SlimeMoldSettings::DiffuseRate = randomFloat(1, 40)(randomEngine);
	SlimeMoldSettings::DecayRate = randomFloat(0.01f, 3)(randomEngine);
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
	return randomPercent(randomEngine);
}
