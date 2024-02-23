#include "SlimeMold.h"

#include <vector>

#include "glad/gl.h"

#include "Settings/SlimeCell.h"
#include "Settings/SpeciesSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"

using std::make_unique;

static constexpr int wrapSettings[] = {
	GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_CLAMP_TO_EDGE
};

static constexpr int filterSettings[] = { GL_NEAREST, GL_LINEAR };

SlimeMold::SlimeMold(int width, int height, unsigned int seed) :
	Simulation(width, height, seed) { };

void SlimeMold::Initialize(int width, int height, unsigned int seed)
{
	Simulation::Initialize(width, height, seed);

	InitializeSettings();
	InitializeTextures();
	InitializeShaders();
	InitializeColony();
	simDrawer = make_unique<SimulationDrawer>();
}

void SlimeMold::InitializeSettings()
{
	totalCells = 0;

	for (SpeciesSettings& species : Colony)
	{
		species.wasEnabled = species.enabled;

		if (species.enabled)
			totalCells += species.cellCount;
	}
}

void SlimeMold::InitializeTextures()
{
	trailTexture = make_unique<Texture>(width, height);
	diffusedTrailTexture = make_unique<Texture>(width, height);

	displayTexture = make_unique<Texture>(
		width, height, GL_RGBA32F,
		wrapSettings[SlimeMoldSettings::SelectedWrap],
		filterSettings[SlimeMoldSettings::SelectedFilter]
	);
}

void SlimeMold::InitializeShaders()
{
	InitializeSlimeShader();
	InitializeDiffuseShader();
	InitializeColorShader();
	InitializeCopyShader();
	ApplyShaderSettings();
}

void SlimeMold::InitializeSlimeShader()
{
	slimeShader = make_unique<ComputeShader>("Slime", totalCells);
	slimeShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	slimeShader->SetInt("height", height);
	slimeShader->SetInt("width", width);
	slimeShader->SetInt("seed", seed);
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

void SlimeMold::ApplyShaderSettings()
{
	std::vector<ShaderSpeciesSettings> enabledSpecies;

	for (const SpeciesSettings& species : Colony)
	{
		if (species.wasEnabled)
			enabledSpecies.push_back(species.ToShaderStruct());
	}

	colonyBuffer = make_unique<ComputeBuffer>(
		enabledSpecies.data(), enabledSpecies.size() * sizeof(ShaderSpeciesSettings)
	);

	slimeShader->SetBufferBinding("colonySettings", colonyBuffer->GetId());

	colorShader->SetBufferBinding("colonySettings", colonyBuffer->GetId());
	colorShader->SetInt("colonySize", static_cast<unsigned int>(enabledSpecies.size()));

	diffuseShader->SetFloat("diffuseRate", SlimeMoldSettings::DiffuseRate);
	diffuseShader->SetFloat("decayRate", SlimeMoldSettings::DecayRate);
}

void SlimeMold::InitializeColony()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<SlimeCell> cells;
	cells.reserve(totalCells);
	int speciesIndex = 0;

	for (const SpeciesSettings& species : Colony)
	{
		if (species.wasEnabled)
		{
			InitializeSpecies(cells, speciesIndex, species.cellCount);
			speciesIndex++;
		}
	}

	cellBuffer = make_unique<ComputeBuffer>(cells.data(), totalCells * sizeof(SlimeCell));
	slimeShader->SetBufferBinding("slimeCells", cellBuffer->GetId());
	slimeShader->SetInt("cellCount", totalCells);
}

void SlimeMold::InitializeSpecies(std::vector<SlimeCell>& cells,
	int speciesIndex, int cellCount) const
{
	for (int i = 0; i < cellCount; i++)
	{
		SlimeCell cell = SlimeCell();
		cell.position[0] = width / 2.0f;
		cell.position[1] = height / 2.0f;

		cell.angle = rand() * SlimeMoldSettings::Tau / RAND_MAX;
		cell.speciesIndex = speciesIndex;

		for (int j = 0; j < Colony.size(); j++)
			cell.speciesMask[j] = static_cast<float>(speciesIndex == j);

		cells.push_back(cell);
	}
}

void SlimeMold::ApplyTextureSettings()
{
	using namespace SlimeMoldSettings;
	displayTexture->SetWrap(wrapSettings[SelectedWrap]);
	displayTexture->SetFilter(filterSettings[SelectedFilter]);
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
	restartPending = false;
}

void SlimeMold::Execute()
{
	slimeShader->SetFloat("time", static_cast<float>(time(nullptr)));
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
