#include "OCSFW.h"

#include "Simulation/SlimeMold/SlimeMold.h"
#include "Interface/ColonyMenu/ColonyMenu.h"
#include "Interface/SlimeMoldMenu/SlimeMoldMenu.h"

int main()
{
	SlimeMold simulation = SlimeMold(1280, 720);

	SlimeMoldMenu moldMenu = SlimeMoldMenu();
	ColonyMenu colonyMenu = ColonyMenu();
	ImGuiWindow* menus[] = { &moldMenu, &colonyMenu };

	OCSFW(&simulation)
		.WithMenus(menus, std::size(menus))
		.WithTitle("Slime Mold")
		.Run();

	return EXIT_SUCCESS;
}
