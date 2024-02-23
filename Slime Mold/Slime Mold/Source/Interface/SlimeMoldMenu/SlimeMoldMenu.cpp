#include "SlimeMoldMenu.h"

#include "imgui/imgui.h"

#include "Simulation/SlimeMold.h"
#include "Settings/MainSettings.h"
#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void SlimeMoldMenu::Initialize()
{
	slimeSim = reinterpret_cast<SlimeMold*>(MainSettings::Sim);
}

void SlimeMoldMenu::Render()
{
	if (Begin("Settings"))
	{
		if (CollapsingHeader("Slime Mold"))
		{
			using namespace SlimeMoldSettings;

			PushItemWidth(-1);
			Checkbox("Show Colony Menu", &ShowColonySettings);
			RenderParameterCombo("Wrap", SelectedWrap, "Mirror\0Border\0Repeat\0Edge\0\0");
			RenderParameterCombo("Filter", SelectedFilter, "Nearest\0Linear\0\0");
			PopItemWidth();
		}
	}

	End();
}

void SlimeMoldMenu::RenderParameterCombo(std::string title,
	int& parameter, const char* labels)
{
	SeparatorText(title.c_str());
	std::string id = "##combo" + title;

	if (Combo(id.c_str(), &parameter, labels))
		slimeSim->ApplyTextureSettings();
}
