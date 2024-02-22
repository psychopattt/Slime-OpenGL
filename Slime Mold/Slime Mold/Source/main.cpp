#include "Simulation/SlimeMold.h"
#include "OCSFW.h"

#include "Interface/SlimeMoldMenu/SlimeMoldMenu.h"
#include "Interface/SlimeSpeciesMenu/SlimeSpeciesMenu.h"

int main()
{
	SlimeMold simulation = SlimeMold(1280, 720);

	SlimeMoldMenu moldMenu = SlimeMoldMenu();
	SlimeSpeciesMenu speciesMenu = SlimeSpeciesMenu();
	ImGuiWindow* menus[] = { &moldMenu, &speciesMenu };

	OCSFW(&simulation, "Slime Mold", menus, std::size(menus));

	return 0;
}
