#include "SlimeCellsMenu.h"

#include <cstdint>

#include "imgui/imgui.h"

#include "Simulation/SlimeMold.h"
#include "Settings/MainSettings.h"
#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeCellsMenu::Render()
{
	if (!SlimeMoldSettings::ShowCellSettings)
		return;

	SetNextWindowPos(ImVec2(591, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(200, -1), ImGuiCond_FirstUseEver);

	if (Begin("Cell Settings", &SlimeMoldSettings::ShowCellSettings))
	{
		PushItemWidth(-1);
		SeparatorText("Cell Count");
		SetItemTooltip("Requires Restart");

		DragInt(
			"##sliderCellCount", &SlimeMoldSettings::CellCount,
			10, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp
		);
		
		RenderParameter("Trail Weight", SlimeMoldSettings::TrailWeight, 50);
		RenderParameter("Diffuse Rate", SlimeMoldSettings::DiffuseRate, 50);
		RenderParameter("Decay Rate", SlimeMoldSettings::DecayRate, 10);
		PopItemWidth();
	}

	End();
}

void SlimeCellsMenu::RenderParameter(std::string label, float& parameter, float max)
{
	SeparatorText(label.c_str());
	std::string id = "##drag" + label;

	bool updatedParameter = DragFloat(
		id.c_str(), &parameter, max / 1000, 0, max,
		"%.3f", ImGuiSliderFlags_AlwaysClamp
	);

	if (updatedParameter)
	{
		SlimeMold* slimeSim = reinterpret_cast<SlimeMold*>(MainSettings::Sim);
		slimeSim->ApplyCellSettings();
	}
}
