module;

#include <memory_resource>
#include <string>
#include <atomic>
#include <iostream>
#include <thread>
#include <functional>
#include <charconv>
#include <sstream>
#include "fmt/core.h"
#include "fmt/color.h"

export module Fibo.MemoryTracker;

namespace fibo
{
	export std::size_t getCurrrentThreadId() 
	{
		try {
			std::ostringstream oss;
			oss << std::this_thread::get_id();
			size_t id{};
			auto str = oss.str();
			auto first = str.data();
			auto last = first + str.size();
			auto [p, ec] = std::from_chars(first, last, id);
			return id;
		}
		catch (std::exception const& ex) {
			_ASSERT(false);
		}
		return 0;
	}	

	export void print(void* p, std::size_t bytes, std::size_t align, std::string_view msg)
	{
		static std::atomic_size_t total = 0;
		static std::atomic_int spaces = 0;
		int indents = spaces;
		fmt::color clr = fmt::color::white;
		if (msg.at(0) == '~') {
			indents = --spaces;
			total.fetch_sub(bytes);
			clr = fmt::color::tomato;
		}
		else {
			indents = spaces++;
			total.fetch_add(bytes);
			clr = fmt::color::green;
		}

		fmt::print(fmt::fg(clr) | fmt::emphasis::italic,
			"{:>{}}[thread {}] [{}] {} - {} - {} - {}\n",
			' ',
			indents * 2,
			getCurrrentThreadId(),
			msg,
			fmt::ptr(p),
			bytes,
			align,
			total);
	}

	class MemoryTracker final: public std::pmr::memory_resource
	{
	private:
		void* do_allocate(std::size_t bytes, std::size_t align) final
		{
			auto p = mUpperStream->allocate(bytes, align);
			print(p, bytes, align, "alloc");
			return p;
		}

		void do_deallocate(void*p, std::size_t bytes, std::size_t align) final
		{
			print(p, bytes, align, "~alloc");
			mUpperStream->deallocate(p, bytes, align);
		}

		bool do_is_equal(std::pmr::memory_resource const& other) const noexcept final
		{
			return mUpperStream->is_equal(other);
		}

	private:
		std::pmr::memory_resource* mUpperStream{ std::pmr::get_default_resource() };
	};

	export std::pmr::memory_resource* getTrackerMemoryResource()
	{
		static fibo::MemoryTracker tracker{};
		return &tracker;
	}
}