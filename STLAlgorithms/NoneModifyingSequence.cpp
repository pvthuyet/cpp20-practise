#include "NoneModifyingSequence.h"
#include "fmt/color.h"
#include <iostream>
#include <numeric>
#include <numbers>
#include <chrono>
#include <execution>

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

	case ranges_all_of:
		ranges_all_of_operation();
		break;

	case ranges_any_of:
		ranges_any_of_operation();
		break;

	case ranges_none_of:
		ranges_none_of_operation();
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
	//using namespace std::literals;
	using stc = std::chrono::steady_clock;
	std::vector<int> v = generateNumber(1'000'000'000);
	auto factor = v.back() + 1;
	fmt::print("Among the numbers {} -> {}. Check all elements are less than {} ?", v.front(), v.back(), factor);
	fmt::print("\n");

	{
		auto start = stc::now();
		auto istrue = std::all_of(v.cbegin(), v.cend(), [factor](auto const& i) {return i < factor; });
		std::chrono::duration<double> diff = stc::now() - start;
		fmt::print("{}, {:.2f} ms,\t{}\n", istrue, 1000 * diff.count(), "C++11 version");
	}
	{
		auto start = stc::now();
		auto istrue = std::all_of(std::execution::seq, v.cbegin(), v.cend(), [factor](auto const& i) {return i < factor; });
		std::chrono::duration<double> diff = stc::now() - start;
		fmt::print("{}, {:.2f} ms,\t{}\n", istrue, 1000 * diff.count(), "C++17 sequenced_policy version");
	}
	{
		auto start = stc::now();
		auto istrue = std::all_of(std::execution::par, v.cbegin(), v.cend(), [factor](auto const& i) {return i < factor; });
		std::chrono::duration<double> diff = stc::now() - start;
		fmt::print("{}, {:.2f} ms,\t{}\n", istrue, 1000 * diff.count(), "C++17 parallel_policy version");
	}
	{
		auto start = stc::now();
		auto istrue = std::all_of(std::execution::par_unseq, v.cbegin(), v.cend(), [factor](auto const& i) {return i < factor; });
		std::chrono::duration<double> diff = stc::now() - start;
		fmt::print("{}, {:.2f} ms,\t{}\n", istrue, 1000 * diff.count(), "C++17 parallel_unsequenced_policy version");
	}
	{
		auto start = stc::now();
		auto istrue = std::all_of(std::execution::unseq, v.cbegin(), v.cend(), [factor](auto const& i) {return i < factor; });
		std::chrono::duration<double> diff = stc::now() - start;
		fmt::print("{}, {:.2f} ms,\t{}\n", istrue, 1000 * diff.count(), "C++20 unsequenced_policy version");
	}

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
	fmt::print("At least one number is divisible by 7: {}\n", any7);
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
	fmt::print("None of them are odd: {}\n", noneOdd);
	fmt::print("=> Done\n\n");
}

void NoneModifyingSequence::ranges_all_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::ranges::all_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");

	auto allEvent = std::ranges::all_of(v, [](auto const& i) {return i % 2 == 0; });

	fmt::print("All element are event: {}\n", allEvent);
	fmt::print("=> Done\n\n");
}

void NoneModifyingSequence::ranges_any_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::ranges::any_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");

	auto any7 = std::ranges::any_of(v, [](auto const& i) {return i % 7 == 0; });
	fmt::print("At least one number is divisible by 7: {}\n", any7);
	fmt::print("=> Done\n\n");
}

void NoneModifyingSequence::ranges_none_of_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::ranges::none_of", 30);

	fmt::print("***** Examples 1 *****\n");
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	fmt::print("Among the numbers: ");
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	fmt::print("\n");

	auto noneOdd = std::ranges::none_of(v, [](auto const& i) {return i % 2 != 0; });
	fmt::print("None of them are odd: {}\n", noneOdd);
	fmt::print("=> Done\n\n");
}

std::vector<int> NoneModifyingSequence::generateNumber(int size) const
{
	std::vector<int> vec(size);
	std::iota(vec.begin(), vec.end(), 1);

	return vec;
}