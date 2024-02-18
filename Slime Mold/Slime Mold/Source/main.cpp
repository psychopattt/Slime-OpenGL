#include "Simulation/Galaxy.h"
#include "OCSFW.h"

int main()
{
	Galaxy simulation = Galaxy(2560, 1440);
	OCSFW(&simulation, "Galaxy");

	return EXIT_SUCCESS;
}
