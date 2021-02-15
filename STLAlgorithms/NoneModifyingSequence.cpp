#include "NoneModifyingSequence.h"
#include "fmt/color.h"
#include <iostream>
#include <numeric>

void NoneModifyingSequence::show(unsigned int index) const
{
	fmt::print(
		"\n+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "Welcome to None-Modifying Sequence Operations", 100);

	switch (index)
	{
	case all_of:
		all_of_operation();
		break;

	default:
		break;
	}
}

void NoneModifyingSequence::all_of_operation() const
{
	fmt::print(
		"\t+{0:-^{2}}+\n"
		"\t|{1: ^{2}}|\n"
		"\t+{0:-^{2}}+\n", "", "std::all_of examples 1", 30);

	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
}
