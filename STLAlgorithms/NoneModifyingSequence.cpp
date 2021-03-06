#include "NoneModifyingSequence.h"
#include "fmt/color.h"
#include <iostream>
#include <numeric>
#include <numbers>
#include <chrono>
#include <execution>
#include "Utils.h"
#include "gsl/assert"

using ICPair = const std::pair<int, char>;
std::ostream& operator << (std::ostream& os, const ICPair& p)
{

}

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

	case for_each:
		for_each_operation();
		break;

	case for_each_n:
		for_each_n_operation();
		break;

	case ranges_for_each:
		ranges_for_each_operation();
		break;

	case ranges_for_each_n:
		ranges_for_each_n_operation();
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
	std::vector<int> v = Utils::generateNumber(1'000);
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

void NoneModifyingSequence::for_each_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::for_each", 30);
	fmt::print("***** Examples 1 *****\n");
	auto v = Utils::generateNumber(20);
	fmt::print("Print out all elments: ");
	std::for_each(v.cbegin(), v.cend(), [](auto const& i) {
		fmt::print("{} ", i);
		});
	fmt::print("\n=> Done\n\n");
}

void NoneModifyingSequence::for_each_n_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::for_each_n", 30);
	fmt::print("***** Examples 1 *****\n");
	auto v = Utils::generateNumber(20);
	fmt::print("Print out 5 elements start from 3 index: ");
	std::for_each_n(v.cbegin() + 3, 5, [](auto const& i) {
		fmt::print("{} ", i);
		});
	fmt::print("\n=> Done\n\n");
}

void NoneModifyingSequence::ranges_for_each_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::ranges::for_each", 30);

	struct Sum {
		int _sum{};
		void operator()(int n) { _sum += n; }
	};

	auto nums = Utils::randomNumber(6, 1, 10);
	auto print = [](auto const& i) { fmt::print(" {}", i); };
	
	fmt::print("Before: ");
	std::ranges::for_each(std::as_const(nums), print);
	fmt::print("\n");

	// Modify each element
	std::ranges::for_each(nums, [](auto& i) { ++i; });
	auto [i, s] = std::ranges::for_each(nums.cbegin(), nums.cend(), Sum{});
	Expects(i == nums.cend());

	fmt::print("After: ");
	std::ranges::for_each(std::as_const(nums), print);
	fmt::print("\nsum: {}\n", s._sum);

	using pair = std::pair<int, std::string>;
	std::vector<pair> pairs = { {1, "one"}, {2, "two"} , {3, "three"} };

	fmt::print("Project the pair::first: ");
	std::ranges::for_each(pairs, print, [](auto const& p) { return p.first; });
	fmt::print("\n");

	fmt::print("Project the pair::second: ");
	std::ranges::for_each(pairs, print, [](auto const& p) { return p.second; });
	fmt::print("\n\n");
}

void NoneModifyingSequence::ranges_for_each_n_operation() const
{
	fmt::print(
		"+{0:-^{2}}+\n"
		"|{1: ^{2}}|\n"
		"+{0:-^{2}}+\n", "", "std::ranges::for_each", 30);

}

