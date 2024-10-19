#pragma once

#include "Inputs/KeyboardHandler/KeyboardHandler.h"

class SlimeMoldKeyboardHandler : public KeyboardHandler
{
	public:
		void HandleKeyboard(struct GLFWwindow* window,
			int key, int scanCode, int action, int mods) override;

	private:
		void ApplyRandomize(int key, int action);
};
