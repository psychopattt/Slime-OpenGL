#pragma once

#include "glad/gl.h"

namespace FilterSettings
{
	inline int SelectedSetting = 0;
	inline constexpr char Labels[] = "Nearest\0Linear\0\0";
	inline constexpr int Values[] = { GL_NEAREST, GL_LINEAR };

	constexpr int GetSelectedValue()
	{
		return Values[SelectedSetting];
	}
}
