#include "Simulation/SlimeMold.h"
#include "OCSFW.h"

#include "Interface/SlimeMoldMenu/SlimeMoldMenu.h"
#include "Interface/SlimeCellsMenu/SlimeCellsMenu.h"
#include "Interface/SlimeSpeciesMenu/SlimeSpeciesMenu.h"

int main()
{
	SlimeMold simulation = SlimeMold(1280, 720);

	SlimeMoldMenu moldMenu = SlimeMoldMenu();
	SlimeCellsMenu cellsMenu = SlimeCellsMenu();
	SlimeSpeciesMenu speciesMenu = SlimeSpeciesMenu();
	ImGuiWindow* menus[] = { &moldMenu, &cellsMenu, &speciesMenu };

	OCSFW(&simulation, "Slime Mold", nullptr, nullptr, menus, std::size(menus));

	return 0;
}
