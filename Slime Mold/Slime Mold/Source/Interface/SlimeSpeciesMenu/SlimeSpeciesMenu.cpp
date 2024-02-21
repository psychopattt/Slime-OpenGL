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
}

void SlimeSpeciesMenu::Render()
{
	if (!SlimeMoldSettings::ShowSpeciesSettings)
		return;

	SetNextWindowPos(ImVec2(270, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(311, -1), ImGuiCond_FirstUseEver);

	if (Begin("Species Settings", &SlimeMoldSettings::ShowSpeciesSettings))
	{
		PushItemWidth(-1);

		if (BeginTabBar("Species"))
		{
			for (int i = 0; i < 3; i++)
				RenderSpeciesTab(i);

			UpdateSpeciesSettings();
			EndTabBar();
		}

		PopItemWidth();
	}

	End();
}

void SlimeSpeciesMenu::RenderSpeciesTab(int speciesId)
{
	SpeciesSettings* species = &AllSpecies[speciesId];
	string tabLabel = "Species " + std::to_string(speciesId + 1);

	if (BeginTabItem(tabLabel.c_str()))
	{
		changesPending = Checkbox("Enabled", &species->enabled) || changesPending;
		SetItemTooltip("Requires Restart");

		if (!species->enabled)
			BeginDisabled();

		RenderParameterDrag("Move Speed", species->moveSpeed, 0, 500);
		RenderParameterDrag("Turn Speed", species->turnSpeed, 0, 20);
		RenderParameterSlider("Sensors Size", species->sensorSize, 1, 10);
		RenderParameterDrag("Sensors Offset", species->sensorOffset, 0, 500);
		RenderParameterDrag("Sensors Angle", species->sensorAngleDegrees, 0, 180);

		SeparatorText("Color");
		changesPending = ColorEdit3(
			"##colorPickerSpecies", species->color
		) || changesPending;

		if (!species->enabled)
			EndDisabled();

		EndTabItem();
	}
}

void SlimeSpeciesMenu::RenderParameterSlider(string label, int& parameter, int min, int max)
{
	SeparatorText(label.c_str());
	string id = "##slider" + label;

	changesPending = SliderInt(
		id.c_str(), &parameter, min, max,
		"%d", ImGuiSliderFlags_AlwaysClamp
	) || changesPending;
}

void SlimeSpeciesMenu::RenderParameterDrag(string label, float& parameter, float min, float max)
{
	SeparatorText(label.c_str());
	string id = "##drag" + label;

	changesPending = DragFloat(
		id.c_str(), &parameter, max / 1000, min, max,
		"%.3f", ImGuiSliderFlags_AlwaysClamp
	) || changesPending;
}

void SlimeSpeciesMenu::UpdateSpeciesSettings()
{
	if (changesPending)
	{
		SlimeMold* slimeSim = reinterpret_cast<SlimeMold*>(MainSettings::Sim);
		slimeSim->ApplySpeciesSettings();
		changesPending = false;
	}
}
