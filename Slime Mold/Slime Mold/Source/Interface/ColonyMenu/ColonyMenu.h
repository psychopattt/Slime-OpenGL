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
		const std::unique_ptr<class LoadColonyModal> loadModal;
		const std::unique_ptr<const class ColonyCodec> colonyCodec;
		
		void RenderLoadModal();
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

		void LoadColony(string colonyString);
		void UpdateSettings();
};
