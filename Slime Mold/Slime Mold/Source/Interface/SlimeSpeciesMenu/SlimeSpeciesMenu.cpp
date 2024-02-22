#include "SlimeSpeciesMenu.h"

#include "imgui/imgui.h"

#include "Simulation/SlimeMold.h"
#include "Settings/MainSettings.h"
#include "Settings/SpeciesSettings.h"
#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeSpeciesMenu::Initialize()
{
	SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel);
	slimeSim = reinterpret_cast<SlimeMold*>(MainSettings::Sim);
}

void SlimeSpeciesMenu::Render()
{
	using SlimeMoldSettings::ShowSpeciesSettings;

	if (!ShowSpeciesSettings)
		return;

	SetNextWindowPos(ImVec2(270, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(311, -1), ImGuiCond_FirstUseEver);
	int windowFlags = slimeSim->IsPendingRestart() ?
		ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags_None;

	if (Begin("Species Settings", &ShowSpeciesSettings, windowFlags))
	{
		PushItemWidth(-1);

		if (BeginTabBar("Species"))
		{
			for (int id = 0; id < AllSpecies.size(); id++)
				RenderSpeciesTab(id);

			UpdateSettings();
			EndTabBar();
		}

		PopItemWidth();
	}

	End();
}

void SlimeSpeciesMenu::RenderSpeciesTab(int speciesId)
{
	SpeciesSettings& species = AllSpecies[speciesId];
	string tabLabel = "Species " + std::to_string(speciesId + 1);

	ImVec2& itemSpacing = GetStyle().ItemSpacing;
	PushStyleVar(ImGuiStyleVar_ItemSpacing,
		ImVec2(itemSpacing.x, itemSpacing.y * 2));
	
	if (BeginTabItem(tabLabel.c_str()))
	{
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

void SlimeSpeciesMenu::RenderEnableSection(struct SpeciesSettings& species)
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

void SlimeSpeciesMenu::RenderCellSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Cell", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SetItemTooltip("Requires Restart");

		if (RenderParameterDrag("Cell Count", species.cellCount, 1, INT32_MAX, 25))
			slimeSim->SetPendingRestart();
	}
}

void SlimeSpeciesMenu::RenderSpeedSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Speed"))
	{
		RenderParameterDrag("Move Speed", species.moveSpeed, -999, 999, 0.01f);
		RenderParameterDrag("Turn Speed", species.turnSpeed, -999, 999, 0.01f);
	}
}

void SlimeSpeciesMenu::RenderTrailSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Trail"))
	{
		RenderParameterDrag("Trail Weight", species.trailWeight, 0, 999, 0.01f);
		RenderParameterDrag("Diffuse Rate", SlimeMoldSettings::DiffuseRate, 0, 999, 0.01f);
		RenderParameterDrag("Decay Rate", SlimeMoldSettings::DecayRate, 0, 999, 0.005f);
	}
}

void SlimeSpeciesMenu::RenderSensorsSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Sensors"))
	{
		RenderParameterSlider("Sensors Size", species.sensorSize, 1, 10);
		RenderParameterDrag("Sensors Offset", species.sensorOffset, -999, 999, 0.01f);
		RenderParameterDrag("Sensors Angle", species.sensorAngleDegrees, -180, 180, 0.1f);
	}
}

void SlimeSpeciesMenu::RenderColorSection(SpeciesSettings& species)
{
	if (CollapsingHeader("Color"))
	{
		changesPending = ColorEdit3(
			"##colorPickerSpecies", species.color
		) || changesPending;
	}
}

bool SlimeSpeciesMenu::RenderParameterSlider(string label, int& parameter, int min, int max)
{
	return RenderParameter(label, false, ImGuiDataType_S32, &parameter, &min, &max);
}

bool SlimeSpeciesMenu::RenderParameterSlider(string label, float& parameter, float min, float max)
{
	return RenderParameter(label, false, ImGuiDataType_Float, &parameter, &min, &max);
}

bool SlimeSpeciesMenu::RenderParameterDrag(string label, int& parameter, int min, int max, float speed)
{
	return RenderParameter(label, true, ImGuiDataType_S32, &parameter, &min, &max, speed);
}

bool SlimeSpeciesMenu::RenderParameterDrag(string label, float& parameter, float min, float max, float speed)
{
	return RenderParameter(label, true, ImGuiDataType_Float, &parameter, &min, &max, speed);
}

bool SlimeSpeciesMenu::RenderParameter(string label, bool isDrag, int dataType, void* parameter,
	const void* min, const void* max, float speed)
{
	SeparatorText(label.c_str());
	string id = "##drag" + label;
	bool parameterModified = false;

	if (isDrag)
	{
		parameterModified = DragScalar(
			id.c_str(), dataType, parameter, speed,
			min, max, NULL, ImGuiSliderFlags_AlwaysClamp
		);
	}
	else
	{
		parameterModified = SliderScalar(
			id.c_str(), dataType, parameter, min,
			max, NULL, ImGuiSliderFlags_AlwaysClamp
		);
	}

	changesPending = changesPending || parameterModified;

	return parameterModified;
}

void SlimeSpeciesMenu::UpdateSettings()
{
	if (changesPending)
	{
		slimeSim->ApplyShaderSettings();
		changesPending = false;
	}
}
