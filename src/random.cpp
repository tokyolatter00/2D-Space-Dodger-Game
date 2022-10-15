
#include "include/random.hpp"

int RandomGenerator::RandInt(int min, int max) {
	// Randomly generate number inbetween min and max

	std::mt19937 rng(Dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);

	return dist6(rng);
}

bool RandomGenerator::ConFlip() {
	// Randomly generate true or false

	return RandInt(1, 2) == 1;
}
