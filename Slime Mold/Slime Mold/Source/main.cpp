#include "Simulation/SlimeMold.h"
#include "OCSFW.h"

int main()
{
	SlimeMold simulation = SlimeMold(1600, 900);
	OCSFW(&simulation, "Slime Mold");

	return 0;
}
