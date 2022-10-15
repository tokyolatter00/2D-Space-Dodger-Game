
#pragma once

#include <random>
#include "types.hpp"

class RandomGenerator {
private:
	std::random_device Dev;
public:
	int RandInt(int min, int max);
	bool ConFlip();
};
