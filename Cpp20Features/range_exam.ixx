#include <ranges>
#include <vector>
#include <algorithm>
#include <iostream>

export module range_exam;

namespace fibo::ranges
{
	struct Foo
	{
		std::pmr::string name;
		unsigned age;
	};

	export void exam1()
	{
		std::pmr::vector<Foo> vec{ {"a", 1}, {"b", 2}, {"c", 3} };
		std::ranges::sort(vec, std::greater{}, &Foo::age);
		for (auto& e : vec) std::cout << e.age << ' ';
		std::cout << std::endl;
	}
}