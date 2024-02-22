#pragma once

#include <string>

#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"

using std::string;

class SlimeSpeciesMenu : public ImGuiWindow
{
	public:
		void Initialize() override;
		void Render() override;

	private:
		bool changesPending = false;

		void RenderSpeciesTab(int speciesId);
		void RenderParameterSlider(string label, int& parameter, int min, int max);
		void RenderParameterDrag(string label, int& parameter, int min, int max);
		void RenderParameterDrag(string label, float& parameter, float min, float max);
		void UpdateSettings();
};
