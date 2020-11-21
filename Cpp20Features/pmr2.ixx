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
		std::string mData{ "just the string is not SSO-string" };
	};

	class Foo
	{
		std::unique_ptr<Bar> mBar{ std::make_unique<Bar>() };
	};

	export void testBar()
	{
		fmt::print("size of Foo: {}\n", sizeof(Foo));
		std::pmr::vector<Foo> vec;
		vec.reserve(2);
		vec.emplace_back();
		vec.emplace_back();
	}

	/*********************************************************************/
	class Bar2
	{
		std::string mData{ "just the string is not SSO-string" };

	public:
		~Bar2()
		{
			int xx = 0;
		}
	};

	class Foo2
	{
		std::unique_ptr<Bar2, fibo::PmrUtils::polymorphic_allocator_delete> mBar;
	public:
		Foo2() :
			mBar(nullptr, { {std::pmr::get_default_resource()} })
		{
			std::pmr::polymorphic_allocator<Bar2> alloc{ std::pmr::get_default_resource() };
			Bar2* const bar = alloc.allocate(1);
			alloc.construct(bar);
			mBar.reset(bar);
		}
	};

	export void testBar2()
	{
		fmt::print("size of Foo2: {}\n", sizeof(Foo2));
		std::pmr::vector<Foo2> vec;
		//vec.reserve(2);
		vec.emplace_back();
		//vec.emplace_back();
	}


	export void testPmr2()
	{
		//testBar();
		testBar2();
	}
}