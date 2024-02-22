#pragma once

#include <memory>
#include <vector>

#include "Simulation/Simulation.h"

using std::unique_ptr;

class Texture;
class ComputeBuffer;
class ComputeShader;

class SlimeMold : public Simulation
{
	public:
		SlimeMold(int width, int height, unsigned int seed = 0);
		void Initialize(int width, int height, unsigned int seed = 0) override;
		void ApplyShaderSettings();
		void Restart() override;
		void Execute() override;
		void Draw() override;
		~SlimeMold();

	private:
		bool simUpdated = false;
		unsigned int totalCells = 0;
		unsigned int enabledSpeciesCount = 1;

		unique_ptr<ComputeShader> slimeShader;
		unique_ptr<ComputeShader> diffuseShader;
		unique_ptr<ComputeShader> colorShader;
		unique_ptr<ComputeShader> copyShader;

		unique_ptr<ComputeBuffer> slimeCells;
		unique_ptr<ComputeBuffer> speciesSettings;

		unique_ptr<Texture> trailTexture;
		unique_ptr<Texture> diffusedTrailTexture;
		unique_ptr<Texture> displayTexture;

		unique_ptr<class SimulationDrawer> simDrawer;

		void InitializeSettings();
		void InitializeTextures();
		void InitializeShaders();
		void InitializeSlimeShader();
		void InitializeDiffuseShader();
		void InitializeColorShader();
		void InitializeCopyShader();
		void InitializeSlimeCells();
		void InitializeCellSpecies(std::vector<struct SlimeCell>& slimeCells,
			int speciesIndex, int cellCount) const;
};
