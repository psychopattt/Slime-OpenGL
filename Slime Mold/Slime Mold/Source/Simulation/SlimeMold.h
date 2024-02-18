#pragma once

#include "Simulation/Simulation.h"

class SlimeMold : public Simulation
{
	public:
		SlimeMold(int width, int height, unsigned int seed = 0);
		void Initialize(int width, int height, unsigned int seed = 0) override;
		void Restart() override;
		void Execute() override;
		void Draw() override;
		~SlimeMold();
};
