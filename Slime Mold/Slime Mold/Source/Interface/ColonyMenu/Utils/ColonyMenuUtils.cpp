#include "ColonyMenuUtils.h"

#include "imgui/imgui.h"

namespace
{
	bool RenderSliderInternal(string label, int dataType,
		void* parameter, const void* min, const void* max)
	{
		ImGui::SeparatorText(label.c_str());
		string id = "##slider" + label;

		return ImGui::SliderScalar(
			id.c_str(), dataType, parameter, min,
			max, NULL, ImGuiSliderFlags_AlwaysClamp
		);
	}

	bool RenderDragInternal(string label, int dataType, void* parameter,
		const void* min, const void* max, float speed)
	{
		ImGui::SeparatorText(label.c_str());
		string id = "##drag" + label;

		return ImGui::DragScalar(
			id.c_str(), dataType, parameter, speed,
			min, max, NULL, ImGuiSliderFlags_AlwaysClamp
		);
	}
}

bool ColonyMenuUtils::RenderSlider(string label, int& parameter, int min, int max)
{
	return RenderSliderInternal(label, ImGuiDataType_S32, &parameter, &min, &max);
}

bool ColonyMenuUtils::RenderSlider(string label, float& parameter, float min, float max)
{
	return RenderSliderInternal(label, ImGuiDataType_Float, &parameter, &min, &max);
}

bool ColonyMenuUtils::RenderDrag(string label, int& parameter, int min, int max, float speed)
{
	return RenderDragInternal(label, ImGuiDataType_S32, &parameter, &min, &max, speed);
}

bool ColonyMenuUtils::RenderDrag(string label, float& parameter, float min, float max, float speed)
{
	return RenderDragInternal(label, ImGuiDataType_Float, &parameter, &min, &max, speed);
}
