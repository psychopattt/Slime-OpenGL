#include "ColonyMenu.h"

#include <vector>

#include "imgui/imgui.h"

#include "Simulation/SlimeMold/SlimeMold.h"
#include "Simulation/ColonyCodec/ColonyCodec.h"
#include "Interface/LoadColonyModal/LoadColonyModal.h"
#include "Settings/MainSettings.h"
#include "Settings/Colony/ColonySettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Utils/ColonyMenuUtils.h"

using namespace ImGui;
using namespace ColonyMenuUtils;

ColonyMenu::ColonyMenu() :
	colonyCodec(std::make_unique<ColonyCodec>()),
	loadModal(std::make_unique<LoadColonyModal>()) { }

void ColonyMenu::Initialize()
{
	SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel);
	slimeSim = reinterpret_cast<SlimeMold*>(MainSettings::Sim);
}

void ColonyMenu::Render()
{
	using SlimeMoldSettings::ShowColonyMenu;

	if (!ShowColonyMenu)
		return;

	RenderLoadModal();
	SetNextWindowPos(ImVec2(270, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(311, -1), ImGuiCond_FirstUseEver);
	int windowFlags = slimeSim->IsPendingRestart() ?
		ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags_None;

	if (Begin("Colony Settings", &ShowColonyMenu, windowFlags))
	{
		RenderWindowPopup();
		PushItemWidth(-1);

		if (BeginTabBar("Species"))
		{
			for (int id = 0; id < Colony.size(); id++)
				RenderSpeciesTab(id);

			EndTabBar();
		}

		PopItemWidth();
	}

	End();
}

void ColonyMenu::RenderLoadModal()
{
	loadModal->Render();

	if (loadModal->HasNewResult())
		LoadColony(loadModal->GetResult());
}

void ColonyMenu::RenderWindowPopup()
{
	if (BeginPopupContextItem())
	{
		if (BeginMenu("Load Preset"))
		{
			for (int i = 0; i < ColonyPresets.size(); i++)
			{
				if (MenuItem(("Colony " + std::to_string(i + 1)).data()))
					LoadColony(ColonyPresets[i]);
			}

			EndMenu();
		}

		if (MenuItem("Load Colony"))
			loadModal->Show();

		if (MenuItem("Copy Colony"))
		{
			SetClipboardText(colonyCodec->EncodeColony(
				vector(Colony.begin(), Colony.end())
			).data());
		}

		EndPopup();
	}
}

void ColonyMenu::RenderSpeciesTab(int speciesId)
{
	SpeciesSettings& species = Colony[speciesId];
	string tabLabel = "Species " + std::to_string(speciesId + 1);

	ImVec2& itemSpacing = GetStyle().ItemSpacing;
	PushStyleVar(ImGuiStyleVar_ItemSpacing,
		ImVec2(itemSpacing.x, itemSpacing.y * 2));
	
	if (BeginTabItem(tabLabel.c_str()))
	{
		RenderTabPopup(speciesId);
		RenderEnableSection(species);

		if (!species.enabled)
			BeginDisabled();

		RenderCellSection(species);
		RenderSpeedSection(species);
		RenderTrailSection(species);
		RenderSensorsSection(species);
		RenderColorSection(species);

		if (!species.enabled)
			EndDisabled();

		EndTabItem();
	}
	else
	{
		PopStyleVar();
	}
}

void ColonyMenu::RenderTabPopup(int speciesId)
{
	if (BeginPopupContextItem())
	{
		RenderCopyMenu(speciesId, true);
		RenderCopyMenu(speciesId, false);
		EndPopup();
	}
}

void ColonyMenu::RenderCopyMenu(int speciesId, bool copyDirection)
{
	if (BeginMenu(copyDirection ? "Copy From" : "Copy To"))
	{
		for (int i = 0; i < Colony.size(); i++)
		{
			int sourceId = copyDirection ? i : speciesId;
			int targetId = copyDirection ? speciesId : i;
			string menuLabel = "Species " + std::to_string(i + 1);

			if (i != speciesId && MenuItem(menuLabel.c_str()))
			{
				SpeciesSettings speciesCopy = SpeciesSettings(Colony[sourceId]);
				speciesCopy.wasEnabled = Colony[targetId].wasEnabled;

				for (int i = 0; i < std::size(speciesCopy.mask); i++)
					speciesCopy.mask[i] = Colony[targetId].mask[i];

				Colony[targetId] = speciesCopy;
				slimeSim->ApplyShaderSettings();
				slimeSim->SetPendingRestart();
			}
		}

		EndMenu();
	}
}

void ColonyMenu::RenderEnableSection(struct SpeciesSettings& species)
{
	if (Checkbox("##checkboxEnabled", &species.enabled))
		slimeSim->SetPendingRestart();

	SetItemTooltip("Requires Restart");
	SameLine();
	
	if (Selectable("Enabled", &species.enabled))
		slimeSim->SetPendingRestart();

	SetItemTooltip("Requires Restart");
	PopStyleVar();
}

void ColonyMenu::RenderCellSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Cell", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SetItemTooltip("Requires Restart");

		if (RenderDrag("Cell Count", species.cellCount, 1, INT32_MAX, 25))
			slimeSim->SetPendingRestart();

		SeparatorText("Spawn Mode");

		if (Combo("##comboSpawnMode", (int*)&species.spawnMode, SpawnModeLabels))
			slimeSim->SetPendingRestart();
	}
}

void ColonyMenu::RenderSpeedSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Speed"))
	{
		if (RenderDrag("Move Speed", species.moveSpeed, -999, 999, 0.01f))
			slimeSim->ApplyShaderSettings();

		if (RenderDrag("Turn Speed", species.turnSpeed, -999, 999, 0.01f))
			slimeSim->ApplyShaderSettings();
	}
}

void ColonyMenu::RenderTrailSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Trail"))
	{
		if (RenderDrag("Trail Weight", species.trailWeight, 0, 999, 0.01f))
			slimeSim->ApplyShaderSettings();

		if (RenderDrag("Diffuse Rate", SlimeMoldSettings::DiffuseRate, 0, 999, 0.01f))
			slimeSim->ApplyShaderSettings();

		if (RenderDrag("Decay Rate", SlimeMoldSettings::DecayRate, 0, 999, 0.005f))
			slimeSim->ApplyShaderSettings();
	}
}

void ColonyMenu::RenderSensorsSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Sensors"))
	{
		if (RenderSlider("Sensors Size", species.sensorSize, 1, 10))
			slimeSim->ApplyShaderSettings();

		if (RenderDrag("Sensors Offset", species.sensorOffset, -999, 999, 0.01f))
			slimeSim->ApplyShaderSettings();

		if (RenderDrag("Sensors Angle", species.sensorAngleDegrees, -180, 180, 0.1f))
			slimeSim->ApplyShaderSettings();
	}
}

void ColonyMenu::RenderColorSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Color", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (RenderColorEdit("Main Color", species.mainColor))
			slimeSim->ApplyShaderSettings();

		if (RenderColorEdit("Edge Color", species.edgeColor))
			slimeSim->ApplyShaderSettings();
	}
}

void ColonyMenu::LoadColony(string colonyString)
{
	std::vector<SpeciesSettings> loadedColony =
		colonyCodec->DecodeColony(colonyString);

	if (!loadedColony.empty())
	{
		for (int speciesId = 0; speciesId < loadedColony.size(); speciesId++)
			Colony[speciesId] = loadedColony[speciesId];

		slimeSim->ApplyShaderSettings();
		slimeSim->SetPendingRestart();
	}
}

ColonyMenu::~ColonyMenu() { }
