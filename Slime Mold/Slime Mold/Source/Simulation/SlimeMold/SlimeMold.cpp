#include "SlimeMold.h"

#include <ctime>
#include <vector>

#include "Interface/Interface.h"
#include "Settings/MainSettings.h"
#include "Settings/Colony/SlimeCell.h"
#include "Settings/Colony/ColonySettings.h"
#include "Settings/Texture/WrapSettings.h"
#include "Settings/Texture/FilterSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Simulation/ColonyBuilder/ColonyBuilder.h"

using std::make_unique;

SlimeMold::SlimeMold(int width, int height) : Simulation(width, height)
{
	colonyBuilder = make_unique<ColonyBuilder>();
};

void SlimeMold::Initialize(int width, int height, unsigned int seed)
{
	Simulation::Initialize(width, height, seed);

	restartPending = false;
	simDrawer = make_unique<SimulationDrawer>();
	displayTexture = make_unique<Texture>(width, height);

	InitializeShaders();
	ApplyShaderSettings();
	ApplyTextureSettings();
}

void SlimeMold::InitializeShaders()
{
	std::vector<float> trailData(4 * width * height);
	trailBuffers = make_unique<DualComputeBuffer>(
		trailData.data(), trailData.size() * sizeof(trailData[0])
	);

	InitializeSlimeShader();
	InitializeDiffuseShader();
	InitializeColorShader();
}

void SlimeMold::InitializeSlimeShader()
{
	std::vector<SlimeCell> cells = colonyBuilder->BuildColony(width, height);
	cellBuffer = make_unique<ComputeBuffer>(cells.data(), cells.size() * sizeof(cells[0]));
	unsigned int cellCount = static_cast<unsigned int>(cells.size());

	slimeShader = make_unique<ComputeShader>("Slime", cellCount);
	slimeShader->SetBufferBinding("slimeCells", cellBuffer->GetId());
	slimeShader->SetUniform("globalSpeed", SlimeMoldSettings::GlobalSpeed);
	slimeShader->SetUniform("cellCount", cellCount);
	slimeShader->SetUniform("size", width, height);
	slimeShader->SetUniform("userSeed", seed);
}

void SlimeMold::InitializeDiffuseShader()
{
	diffuseShader = make_unique<ComputeShader>("Diffuse", width, height);
	diffuseShader->SetUniform("globalSpeed", SlimeMoldSettings::GlobalSpeed);
	diffuseShader->SetUniform("size", width, height);
}

void SlimeMold::InitializeColorShader()
{
	colorShader = make_unique<ComputeShader>("Color", width, height);
	colorShader->SetTextureBinding("displayTexture", displayTexture->GetId());
	colorShader->SetUniform("size", width, height);
}

void SlimeMold::ApplyShaderSettings()
{
	std::vector<ShaderSpeciesSettings> enabledSpecies;

	for (const SpeciesSettings& species : Colony)
	{
		if (species.wasEnabled)
			enabledSpecies.push_back(species.ToShaderStruct());
	}

	colonyBuffer = make_unique<ComputeBuffer>(
		enabledSpecies.data(), enabledSpecies.size() * sizeof(enabledSpecies[0])
	);

	slimeShader->SetBufferBinding("colonySettings", colonyBuffer->GetId());

	colorShader->SetBufferBinding("colonySettings", colonyBuffer->GetId());
	colorShader->SetUniform("colonySize", static_cast<unsigned int>(enabledSpecies.size()));

	diffuseShader->SetUniform("diffuseRate", SlimeMoldSettings::DiffuseRate);
	diffuseShader->SetUniform("decayRate", SlimeMoldSettings::DecayRate);
}

void SlimeMold::ApplyTextureSettings()
{
	displayTexture->SetWrap(WrapSettings::GetSelectedValue());
	displayTexture->SetFilter(FilterSettings::GetSelectedValue());
}

void SlimeMold::RandomizeColony()
{
	colonyBuilder->RandomizeColony();
	Simulation::Restart(
		MainSettings::PendingSimSize[0],
		MainSettings::PendingSimSize[1],
		MainSettings::PendingSimSeed
	);

	MainSettings::Gui->NotifyRestart();
}

bool SlimeMold::IsPendingRestart() const
{
	return restartPending;
}

void SlimeMold::SetPendingRestart()
{
	restartPending = true;
}

void SlimeMold::Restart()
{
	Simulation::Initialize();
}

void SlimeMold::Execute()
{
	drawPending = true;
	trailBuffers->Swap();

	unsigned int seconds = static_cast<unsigned int>(time(nullptr));
	slimeShader->SetBufferBinding("trailBuffer", trailBuffers->GetId(0));
	slimeShader->SetUniform("seed", seconds);
	slimeShader->Execute();

	diffuseShader->SetBufferBinding("trailBuffer", trailBuffers->GetId(0));
	diffuseShader->SetBufferBinding("diffusedTrailBuffer", trailBuffers->GetId(1));
	diffuseShader->Execute();
}

void SlimeMold::Draw()
{
	if (drawPending)
	{
		colorShader->SetBufferBinding("trailBuffer", trailBuffers->GetId(1));
		colorShader->Execute();
		drawPending = false;
	}
	
	simDrawer->Draw(displayTexture.get());
}

SlimeMold::~SlimeMold() { }
