#include "Simulation/SlimeMold.h"
#include "OCSFW.h"

int main()
{
	SlimeMold simulation = SlimeMold(1280, 720);
	OCSFW(&simulation, "Slime Mold");

	return 0;
}
