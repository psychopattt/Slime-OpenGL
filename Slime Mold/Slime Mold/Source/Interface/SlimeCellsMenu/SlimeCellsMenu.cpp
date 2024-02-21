#include "SlimeCellsMenu.h"

#include <cstdint>

#include "imgui/imgui.h"

#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeCellsMenu::Render()
{
	if (!SlimeMoldSettings::ShowCellSettings)
		return;

	SetNextWindowPos(ImVec2(270, 220), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(200, -1), ImGuiCond_FirstUseEver);

	if (Begin("Cell Settings", &SlimeMoldSettings::ShowCellSettings))
	{
		SeparatorText("Cell Count");

		DragInt(
			"##sliderCellCount", &SlimeMoldSettings::CellCount,
			10, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp
		);
	}

	End();
}
