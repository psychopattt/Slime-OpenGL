#pragma once

#include <memory>

#include "Simulation/Simulation.h"

using std::unique_ptr;

class ComputeBuffer;
class ComputeShader;

class SlimeMold : public Simulation
{
	public:
		SlimeMold(int width, int height);
		void Initialize(int width, int height, unsigned int seed = 0) override;
		void ApplyShaderSettings();
		void ApplyTextureSettings();
		void RandomizeColony();
		bool IsPendingRestart() const;
		void SetPendingRestart();
		void Restart() override;
		void Execute() override;
		void Draw() override;
		~SlimeMold();

	private:
		bool drawPending = false;
		bool restartPending = false;

		unique_ptr<ComputeShader> slimeShader;
		unique_ptr<ComputeShader> diffuseShader;
		unique_ptr<ComputeShader> colorShader;

		unique_ptr<ComputeBuffer> cellBuffer;
		unique_ptr<ComputeBuffer> colonyBuffer;
		unique_ptr<class DualComputeBuffer> trailBuffers;
		unique_ptr<class Texture> displayTexture;

		unique_ptr<class SimulationDrawer> simDrawer;
		unique_ptr<class ColonyBuilder> colonyBuilder;

		void InitializeShaders();
		void InitializeSlimeShader();
		void InitializeDiffuseShader();
		void InitializeColorShader();
};
