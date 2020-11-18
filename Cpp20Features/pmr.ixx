#include <iostream>
#include <memory_resource>
#include <vector>

export module Fibo.Pmr;

struct Foo
{
	int x{ 0 };
	int y{ 0 };
	Foo(int v) : x{ v }, y{ v } {}
};

export void testMonotoicBuffer()
{
	constexpr size_t sz = 64;
	std::byte buf[sz] = {};
	std::pmr::monotonic_buffer_resource rsrc(&buf, sz);

	std::pmr::vector<Foo> vec{ &rsrc };
	for (int i = 1; i < 3; ++i) {
		vec.emplace_back(i);
	}

	std::cout << "value from stack buf:\n";
	for (int i = 0; i < sz; i += sizeof(Foo)) {
		Foo* p = static_cast<Foo*>((void*) &buf[i]);
		std::cout << &(p->x) << "# " << p->x << std::endl;
	}

	std::cout << "\nvalue from vector:\n";
	for (auto&& e : vec) {
		std::cout << &(e.x) << "# " << e.x << std::endl;
	}
}