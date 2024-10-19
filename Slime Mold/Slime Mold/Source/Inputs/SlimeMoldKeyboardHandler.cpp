#include "SlimeMoldKeyboardHandler.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Simulation/SlimeMold/SlimeMold.h"
#include "Settings/MainSettings.h"

void SlimeMoldKeyboardHandler::HandleKeyboard(GLFWwindow* window,
	int key, int scanCode, int action, int mods)
{
	KeyboardHandler::HandleKeyboard(window, key, scanCode, action, mods);

	if (!ImGui::GetIO().WantCaptureKeyboard)
		ApplyRandomize(key, action);
}

void SlimeMoldKeyboardHandler::ApplyRandomize(int key, int action)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		reinterpret_cast<SlimeMold*>(MainSettings::Sim)->RandomizeColony();
}
