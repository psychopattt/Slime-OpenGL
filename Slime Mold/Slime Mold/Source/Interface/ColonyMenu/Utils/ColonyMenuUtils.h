#pragma once

#include <string>

using std::string;

namespace ColonyMenuUtils
{
	bool RenderSlider(string label, int& parameter, int min, int max);
	bool RenderSlider(string label, float& parameter, float min, float max);
	bool RenderDrag(string label, int& parameter, int min, int max, float speed);
	bool RenderDrag(string label, float& parameter, float min, float max, float speed);
	bool RenderColorEdit(string label, float* color);
};
