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
		void ApplyTextureSettings();
		bool IsPendingRestart() const;
		void SetPendingRestart();
		void Restart() override;
		void Execute() override;
		void Draw() override;
		~SlimeMold();

	private:
		bool simUpdated = false;
		bool restartPending = false;
		unsigned int totalCells = 0;

		unique_ptr<ComputeShader> slimeShader;
		unique_ptr<ComputeShader> diffuseShader;
		unique_ptr<ComputeShader> colorShader;
		unique_ptr<ComputeShader> copyShader;

		unique_ptr<ComputeBuffer> cellBuffer;
		unique_ptr<ComputeBuffer> colonyBuffer;

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
		void InitializeColony();
		void InitializeSpecies(std::vector<struct SlimeCell>& cells,
			int speciesIndex, int cellCount) const;
};
