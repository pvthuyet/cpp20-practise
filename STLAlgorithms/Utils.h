#pragma once

#include <vector>

namespace Utils
{
	std::vector<int> generateNumber(int size);

	std::vector<int> randomNumber(int size, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max());
};

