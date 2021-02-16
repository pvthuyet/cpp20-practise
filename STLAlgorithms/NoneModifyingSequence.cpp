#include "NoneModifyingSequence.h"
#include "fmt/color.h"
#include <iostream>
#include <numeric>

void NoneModifyingSequence::name()
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "Welcome to None-Modifying Sequence Operations", 100);
}

void NoneModifyingSequence::show(unsigned int index) const
{
	switch (index)
	{
	case all_of:
		all_of_operation();
		break;

	case any_of:
		any_of_operation();
		break;

	case none_of:
		none_of_operation();
		break;

	default:
		break;
	}
}

void NoneModifyingSequence::all_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::all_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");
	auto allEvent = std::all_of(v.cbegin(), v.cend(), [](auto const& i) {return i % 2 == 0; });
	fmt::print("All element are event: {}\n", allEvent);
	fmt::print("=> Done\n\n");
}

void NoneModifyingSequence::any_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::any_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");
	auto any7 = std::any_of(v.cbegin(), v.cend(), [](auto const& i) {return i % 7 == 0; });
	fmt::print("There is an element multiple of 7: {}\n", any7);
	fmt::print("=> Done\n\n");
}

void NoneModifyingSequence::none_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::none_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");
	auto noneOdd = std::none_of(v.cbegin(), v.cend(), [](auto const& i) {return i % 2 != 0; });
	fmt::print("None element is odd: {}\n", noneOdd);
	fmt::print("=> Done\n\n");
}