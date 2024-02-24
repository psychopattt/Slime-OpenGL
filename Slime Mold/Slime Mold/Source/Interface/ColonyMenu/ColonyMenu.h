#pragma once

#include <memory>
#include <string>

#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"

using std::string;

struct SpeciesSettings;

class ColonyMenu : public ImGuiWindow
{
	public:
		ColonyMenu();
		void Initialize() override;
		void Render() override;
		~ColonyMenu();

	private:
		bool changesPending = false;
		class SlimeMold* slimeSim = nullptr;
		const std::unique_ptr<class ColonyCodec> colonyCodec;
		
		void RenderWindowPopup();
		void RenderSpeciesTab(int speciesId);
		void RenderTabPopup(int speciesId);
		void RenderCopyMenu(int speciesId, bool copyDirection);
		void RenderEnableSection(SpeciesSettings& species);
		void RenderCellSection(SpeciesSettings& species);
		void RenderSpeedSection(SpeciesSettings& species);
		void RenderTrailSection(SpeciesSettings& species);
		void RenderSensorsSection(SpeciesSettings& species);
		void RenderColorSection(SpeciesSettings& species);

		bool RenderParameterSlider(string label, int& parameter, int min, int max);
		bool RenderParameterSlider(string label, float& parameter, float min, float max);
		bool RenderParameterDrag(string label, int& parameter, int min, int max, float speed);
		bool RenderParameterDrag(string label, float& parameter, float min, float max, float speed);
		bool RenderParameter(string label, bool isDrag, int dataType, void* parameter,
			const void* min, const void* max, float speed = 0);

		void LoadColony(string colonyString);
		void UpdateSettings();
};
