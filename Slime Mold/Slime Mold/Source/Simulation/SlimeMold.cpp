#include "SlimeMold.h"

#include <vector>

#include "Settings/SlimeCell.h"
#include "Settings/SpeciesSettings.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"

using std::make_unique;

SlimeMold::SlimeMold(int width, int height, unsigned int seed) :
	Simulation(width, height, seed) { };

void SlimeMold::Initialize(int width, int height, unsigned int seed)
{
	InitializeTextures();
	InitializeShaders();
	InitializeSlimeCells();
	InitializeSpeciesSettings();
	simDrawer = make_unique<SimulationDrawer>();
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
	slimeShader = make_unique<ComputeShader>("Slime", 100, 1);
	slimeShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	slimeShader->SetFloat("trailWeight", 80);
	slimeShader->SetInt("height", height);
	slimeShader->SetInt("width", width);
	slimeShader->SetInt("seed", seed);
}

void SlimeMold::InitializeDiffuseShader()
{
	diffuseShader = make_unique<ComputeShader>("Diffuse", width, height);
	diffuseShader->SetTextureBinding("trailTexture", trailTexture->GetId());
	diffuseShader->SetTextureBinding("diffusedTrailTexture", diffusedTrailTexture->GetId());
	diffuseShader->SetFloat("diffuseRate", 1);
	diffuseShader->SetFloat("decayRate", 1);
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

void SlimeMold::InitializeSlimeCells()
{
	constexpr float tau = 6.2831853072f;
	srand(static_cast<unsigned int>(time(nullptr)));
	std::vector<SlimeCell> cells(100);

	for (int i = 0; i < cells.size(); i++)
	{
		cells[i].position[0] = width / 2.0f;
		cells[i].position[1] = height / 2.0f;

		cells[i].angle = static_cast<float>(rand()) / RAND_MAX * tau;
		cells[i].speciesIndex = 0;

		for (int j = 0; j < 3; j++)
			cells[i].speciesMask[j] = static_cast<float>(cells[i].speciesIndex == j);
	}

	slimeCells = make_unique<ComputeBuffer>(cells.data(), cells.size() * sizeof(SlimeCell));
	slimeShader->SetInt("slimeCellCount", static_cast<unsigned int>(cells.size()));
	slimeShader->SetBufferBinding("slimeCells", slimeCells->GetId());
}

void SlimeMold::InitializeSpeciesSettings()
{
	std::vector<SpeciesSettings> settings(1);
	settings[0].moveSpeed = 20;
	settings[0].turnSpeed = 2;
	settings[0].sensorSize = 1;
	settings[0].sensorOffset = 35;
	settings[0].sensorAngleDegrees = 30;
	settings[0].color[0] = 1;
	settings[0].color[1] = 1;
	settings[0].color[2] = 1;

	speciesSettings = make_unique<ComputeBuffer>(settings.data(), settings.size() * sizeof(SpeciesSettings));
	slimeShader->SetBufferBinding("speciesSettings", speciesSettings->GetId());

	colorShader->SetBufferBinding("speciesSettings", speciesSettings->GetId());
	colorShader->SetInt("speciesCount", static_cast<unsigned int>(settings.size()));
}

void SlimeMold::Restart()
{
	
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
