#pragma once

#include "glad/gl.h"

namespace WrapSettings
{
	inline int SelectedSetting = 0;
	inline constexpr char Labels[] = "Mirror\0Border\0Repeat\0Edge\0\0";
	inline constexpr int Values[] = {
		GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_CLAMP_TO_EDGE
	};

	constexpr int GetSelectedValue()
	{
		return Values[SelectedSetting];
	}
}
