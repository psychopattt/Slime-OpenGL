#pragma once

#include <string>

#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"

class SlimeCellsMenu : public ImGuiWindow
{
	public:
		void Render() override;

	private:
		void RenderParameter(std::string label, float& parameter, float max);
};
