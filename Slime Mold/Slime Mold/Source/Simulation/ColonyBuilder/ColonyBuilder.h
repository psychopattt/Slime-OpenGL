#pragma once

#include <vector>
#include <random>

struct SlimeCell;
struct SpeciesSettings;
enum SpawnMode : int;

class ColonyBuilder
{
	public:
		ColonyBuilder();
		std::vector<SlimeCell> BuildColony(unsigned int width,
			unsigned int height, int totalCells);

	private:
		float missileAngle = 0;
		float center[2] = { 0, 0 };
		unsigned int smallestDimension = 0;

		std::mt19937 randomEngine;
		std::uniform_real_distribution<float> randomDistribution;

		void BuildSpecies(std::vector<SlimeCell>& cells,
			int speciesIndex, const SpeciesSettings& cellCount,
			unsigned int width, unsigned int height);

		void GenerateCellSpawn(SlimeCell& cell, SpawnMode spawnMode,
			unsigned int width, unsigned int height);

		void RandomCirclePosition(float* position, float radius);
		float Random01();
};
