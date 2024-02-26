#include "SlimeMoldMenu.h"

#include "imgui/imgui.h"

#include "Simulation/SlimeMold/SlimeMold.h"
#include "Settings/MainSettings.h"
#include "Settings/SlimeMoldSettings.h"
#include "Settings/Texture/WrapSettings.h"
#include "Settings/Texture/FilterSettings.h"

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
			PushItemWidth(-1);
			Checkbox("Show Colony Menu", &SlimeMoldSettings::ShowColonyMenu);
			RenderParameterCombo("Wrap", WrapSettings::SelectedSetting, WrapSettings::Labels);
			RenderParameterCombo("Filter", FilterSettings::SelectedSetting, FilterSettings::Labels);
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
