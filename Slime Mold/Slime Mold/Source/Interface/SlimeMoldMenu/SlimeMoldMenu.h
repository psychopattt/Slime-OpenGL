#pragma once

#include <string>

#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"

class SlimeMoldMenu : public ImGuiWindow
{
	public:
		void Initialize() override;
		void Render() override;

	private:
		class SlimeMold* slimeSim = nullptr;

		void RenderParameterCombo(std::string title,
			int& parameter, const char* labels);
};
