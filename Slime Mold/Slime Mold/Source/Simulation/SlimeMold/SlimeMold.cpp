#include "SlimeMold.h"

#include <ctime>
#include <vector>

#include "Settings/Colony/SlimeCell.h"
#include "Settings/Colony/ColonySettings.h"
#include "Settings/Texture/WrapSettings.h"
#include "Settings/Texture/FilterSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
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

	InitializeSettings();
	InitializeTextures();
	InitializeShaders();
	InitializeColony();
	ApplyShaderSettings();
	ApplyTextureSettings();
	simDrawer = make_unique<SimulationDrawer>();
}

void SlimeMold::InitializeSettings()
{
	restartPending = false;
	totalCells = 0;

	for (SpeciesSettings& species : Colony)
	{
		if (species.enabled)
			totalCells += species.cellCount;
	}
}

void SlimeMold::InitializeTextures()
{
	trailTexture = make_unique<Texture>(width, height);
	diffusedTrailTexture = make_unique<Texture>(width, height);
	displayTexture = make_unique<Texture>(width, height);
}

void SlimeMold::InitializeShaders()
{
	InitializeSlimeShader();
	InitializeDiffuseShader();
	InitializeColorShader();
	InitializeCopyShader();
}

void SlimeMold::InitializeSlimeShader()
{
	slimeShader = make_unique<ComputeShader>("Slime", totalCells);
	slimeShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	slimeShader->SetInt("userSeed", seed);
	slimeShader->SetInt("height", height);
	slimeShader->SetInt("width", width);
}

void SlimeMold::InitializeDiffuseShader()
{
	diffuseShader = make_unique<ComputeShader>("Diffuse", width, height);
	diffuseShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	diffuseShader->SetTextureBinding("diffusedTrailTexture", diffusedTrailTexture->GetId());
	diffuseShader->SetInt("height", height);
	diffuseShader->SetInt("width", width);
}

void SlimeMold::InitializeColorShader()
{
	colorShader = make_unique<ComputeShader>("Color", width, height);
	colorShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	colorShader->SetTextureBinding("displayTexture", displayTexture->GetId());
	colorShader->SetInt("height", height);
	colorShader->SetInt("width", width);
}

void SlimeMold::InitializeCopyShader()
{
	copyShader = make_unique<ComputeShader>("Copy", width, height);
	copyShader->SetTextureBinding("source", diffusedTrailTexture->GetId());
	copyShader->SetTextureBinding("target", trailTexture->GetId());
}

void SlimeMold::InitializeColony()
{
	std::vector<SlimeCell> cells = colonyBuilder->BuildColony(width, height, totalCells);
	cellBuffer = make_unique<ComputeBuffer>(cells.data(), cells.size() * sizeof(cells[0]));
	slimeShader->SetInt("cellCount", static_cast<unsigned int>(cells.size()));
	slimeShader->SetBufferBinding("slimeCells", cellBuffer->GetId());
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
	colorShader->SetInt("colonySize", static_cast<unsigned int>(enabledSpecies.size()));

	diffuseShader->SetFloat("diffuseRate", SlimeMoldSettings::DiffuseRate);
	diffuseShader->SetFloat("decayRate", SlimeMoldSettings::DecayRate);
}

void SlimeMold::ApplyTextureSettings()
{
	displayTexture->SetWrap(WrapSettings::GetSelectedValue());
	displayTexture->SetFilter(FilterSettings::GetSelectedValue());
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
	unsigned int seconds = static_cast<unsigned int>(time(nullptr));
	slimeShader->SetInt("seed", seconds);
	slimeShader->Execute();
	diffuseShader->Execute();
	copyShader->Execute();
	simUpdated = true;
}

void SlimeMold::Draw()
{
	if (simUpdated)
	{
		colorShader->Execute();
		simUpdated = false;
	}
	
	simDrawer->Draw(displayTexture.get());
}

SlimeMold::~SlimeMold() { }
