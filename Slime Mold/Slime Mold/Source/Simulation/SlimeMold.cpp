#include "SlimeMold.h"

#include <vector>

#include "Settings/SlimeCell.h"
#include "Settings/SpeciesSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"

using std::make_unique;

SlimeMold::SlimeMold(int width, int height, unsigned int seed) :
	Simulation(width, height, seed) { };

void SlimeMold::Initialize(int width, int height, unsigned int seed)
{
	Simulation::Initialize(width, height, seed);

	InitializeSettings();
	InitializeTextures();
	InitializeShaders();
	InitializeSlimeCells();
	simDrawer = make_unique<SimulationDrawer>();
}

void SlimeMold::InitializeSettings()
{
	totalCells = 0;
	enabledSpeciesCount = 0;

	for (int speciesId = 0; speciesId < AllSpecies.size(); speciesId++)
	{
		EnabledSpecies[speciesId] = AllSpecies[speciesId].enabled;

		if (EnabledSpecies[speciesId])
		{
			totalCells += AllSpecies[speciesId].cellCount;
			enabledSpeciesCount++;
		}
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
	ApplyShaderSettings();
}

void SlimeMold::InitializeSlimeShader()
{
	slimeShader = make_unique<ComputeShader>("Slime", totalCells, 1);
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
	std::vector<SpeciesSettings> enabledSpecies;

	for (int i = 0; i < AllSpecies.size(); i++)
	{
		if (EnabledSpecies[i])
			enabledSpecies.push_back(AllSpecies[i]);
	}

	speciesSettings = make_unique<ComputeBuffer>(enabledSpecies.data(), enabledSpeciesCount * sizeof(SpeciesSettings));
	slimeShader->SetBufferBinding("speciesSettings", speciesSettings->GetId());

	colorShader->SetBufferBinding("speciesSettings", speciesSettings->GetId());
	colorShader->SetInt("speciesCount", enabledSpeciesCount);

	diffuseShader->SetFloat("diffuseRate", SlimeMoldSettings::DiffuseRate);
	diffuseShader->SetFloat("decayRate", SlimeMoldSettings::DecayRate);
}

void SlimeMold::InitializeSlimeCells()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<SlimeCell> cells;
	cells.reserve(totalCells);
	int speciesIndex = 0;

	for (int speciesId = 0; speciesId < AllSpecies.size(); speciesId++)
	{
		if (EnabledSpecies[speciesId])
		{
			InitializeCellSpecies(cells, speciesIndex, AllSpecies[speciesId].cellCount);
			speciesIndex++;
		}
	}

	slimeCells = make_unique<ComputeBuffer>(cells.data(), totalCells * sizeof(SlimeCell));
	slimeShader->SetBufferBinding("slimeCells", slimeCells->GetId());
	slimeShader->SetInt("slimeCellCount", totalCells);
}

void SlimeMold::InitializeCellSpecies(std::vector<SlimeCell>& slimeCells,
	int speciesIndex, int cellCount) const
{
	for (int i = 0; i < cellCount; i++)
	{
		SlimeCell cell = SlimeCell();
		cell.position[0] = width / 2.0f;
		cell.position[1] = height / 2.0f;

		cell.angle = rand() * SlimeMoldSettings::Tau / RAND_MAX;
		cell.speciesIndex = speciesIndex;

		for (int j = 0; j < AllSpecies.size(); j++)
			cell.speciesMask[j] = static_cast<float>(speciesIndex == j);

		slimeCells.push_back(cell);
	}
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
