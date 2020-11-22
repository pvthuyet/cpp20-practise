module;
#include <string>
#include <memory>
#include <memory_resource>
#include "fmt/color.h"
import Fibo.PmrUtils;


export module Fibo.Pmr2;


namespace fibo::pmr2
{
	class Bar
	{
		std::string mData{ "data" };
	};

	class Foo
	{
		std::unique_ptr<Bar> mBar{ std::make_unique<Bar>() };
	};

	export void testBar()
	{
		fmt::print("size of Foo: {}\n", sizeof(Foo));
		std::pmr::vector<Foo> vec;
		//vec.reserve(2);
		vec.emplace_back();
		vec.emplace_back();
	}

	/*********************************************************************/
	class Bar2
	{
		std::string mData{ "data" };
	};

	class Foo2
	{
		std::unique_ptr<Bar2, fibo::PmrUtils::polymorphic_allocator_delete> mBar;
	public:
		Foo2() :
			mBar(nullptr,
				{ {std::pmr::get_default_resource()} })
		{
			std::pmr::polymorphic_allocator<Bar2> alloc{ std::pmr::get_default_resource() };
			Bar2* const bar = alloc.allocate(1);
			try {
				alloc.construct(bar);
			}
			catch (...) {
				alloc.deallocate(bar, 1);
				throw;
			}
			mBar.reset(bar);
		}
	};

	export void testBar2()
	{
		fmt::print("size of Foo2: {}\n", sizeof(Foo2));
		std::pmr::vector<Foo2> vec;
		//vec.reserve(1);
		vec.emplace_back();
		vec.emplace_back();
	}


	/*********************************************************************/
	class Bar3
	{
		std::pmr::string mData{ "data" };
	};

	class Foo3
	{
		std::unique_ptr<Bar3, fibo::PmrUtils::polymorphic_allocator_delete> mBar;
	public:
		Foo3() :
			mBar(nullptr,
				{ {std::pmr::get_default_resource()} })
		{
			std::pmr::polymorphic_allocator<Bar3> alloc{ std::pmr::get_default_resource() };
			Bar3* const bar = alloc.allocate(1);
			try {
				alloc.construct(bar);
			}
			catch (...) {
				alloc.deallocate(bar, 1);
				throw;
			}
			mBar.reset(bar);
		}
	};

	export void testBar3()
	{
		fmt::print("size of Foo3: {}\n", sizeof(Foo3));
		std::pmr::vector<Foo3> vec;
		//vec.reserve(1);
		vec.emplace_back();
		vec.emplace_back();
	}

	/*********************************************************************/
	class Bar4
	{
		std::pmr::string mData{ "data" };
	};
	class Foo4
	{
		std::unique_ptr<Bar4, fibo::PmrUtils::default_polymorphic_allocator_delete> mBar;
	public:
		Foo4()
		{
			std::pmr::polymorphic_allocator<Bar4> alloc{ std::pmr::get_default_resource() };
			Bar4* const bar = alloc.allocate(1);
			try {
				alloc.construct(bar);
			}
			catch (...) {
				alloc.deallocate(bar, 1);
				throw;
			}
			mBar.reset(bar);
		}
	};

	export void testBar4()
	{
		fmt::print("size of Foo4: {}\n", sizeof(Foo4));
		std::pmr::vector<Foo4> vec;
		//vec.reserve(1);
		vec.emplace_back();
		vec.emplace_back();
	}

	export void testPmr2()
	{
		testBar();
		testBar2();
		testBar3();
		testBar4();
	}
}