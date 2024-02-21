#include "SlimeMoldMenu.h"

#include "imgui/imgui.h"

#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeMoldMenu::Render()
{
	if (Begin("Settings"))
	{
		if (CollapsingHeader("Slime Mold"))
		{
			Checkbox("Show Cells Menu", &SlimeMoldSettings::ShowCellSettings);
			Checkbox("Show Species Menu", &SlimeMoldSettings::ShowSpeciesSettings);
		}
	}

	End();
}
