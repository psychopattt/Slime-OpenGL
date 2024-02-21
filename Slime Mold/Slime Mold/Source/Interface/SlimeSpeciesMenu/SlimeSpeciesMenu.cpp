#include "SlimeSpeciesMenu.h"

#include "imgui/imgui.h"

#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeSpeciesMenu::Render()
{
	if (!SlimeMoldSettings::ShowSpeciesSettings)
		return;

	SetNextWindowPos(ImVec2(270, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(220, 200), ImGuiCond_FirstUseEver);

	if (Begin("Species Settings", &SlimeMoldSettings::ShowSpeciesSettings))
	{
		
	}

	End();
}
