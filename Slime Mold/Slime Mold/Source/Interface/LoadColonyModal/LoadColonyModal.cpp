#include "LoadColonyModal.h"

#include "imgui/imgui.h"

#include "Settings/SlimeMoldSettings.h"

using namespace ImGui;

void LoadColonyModal::Render()
{
	if (SlimeMoldSettings::ShowLoadModal)
		OpenPopup("Load Colony");

	if (BeginPopupModal("Load Colony", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		PushItemWidth(-1);

		if (InputTextWithHint("##textColony", "Insert Colony String",
			input, std::size(input), ImGuiInputTextFlags_EnterReturnsTrue))
			ExitPopup(true);

		if (Button("Load", ImVec2(150, 0)))
			ExitPopup(true);

		SameLine();

		if (Button("Cancel", ImVec2(150, 0)) || IsKeyDown(ImGuiKey_Escape))
			ExitPopup(false);

		PopItemWidth();
		EndPopup();
	}
}

void LoadColonyModal::ExitPopup(bool hasNewResult)
{
	this->hasNewResult = hasNewResult;
	result = hasNewResult ? input : "";
	SlimeMoldSettings::ShowLoadModal = false;
	CloseCurrentPopup();
	input[0] = '\0';
}

bool LoadColonyModal::HasNewResult() const
{
	return hasNewResult;
}

const string& LoadColonyModal::GetResult()
{
	hasNewResult = false;
	return result;
}
