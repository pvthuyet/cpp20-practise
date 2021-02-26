#include "Utils.h"
#include <numeric>
#include <random>

namespace Utils
{
	std::vector<int> generateNumber(int size)
	{
		std::vector<int> vec(size);
		std::iota(vec.begin(), vec.end(), 1);
		return vec;
	}

	std::vector<int> randomNumber(int size, int min, int max)
	{
		std::vector<int> result(size);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(min, max);
		for (int i = 0; i < size; ++i) {
			result[i] = distrib(gen);
		}
		return result;
	}
}