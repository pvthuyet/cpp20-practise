module;

#include <memory_resource>
#include <string>
#include <atomic>
#include <iostream>
#include <thread>
#include <functional>
#include "fmt/core.h"
#include "fmt/color.h"

export module Fibo.MemoryTracker;

namespace fibo
{
	std::size_t getThreadId() 
	{
		static std::atomic<std::size_t> threadIndex{ 0 };
		thread_local std::size_t id = threadIndex;
		threadIndex++;
		return id;
	}

	class MemoryTracker final: public std::pmr::memory_resource
	{
		void print(void* p, std::size_t bytes, std::size_t align, std::string_view msg)
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
				"{:>{}}[thread {}] {} {} - {} - {}\n", 
				' ', 
				indents * 2,
				getThreadId(),
				msg, 
				bytes, 
				align, 
				total);
		}

	private:
		virtual void* do_allocate(std::size_t bytes, std::size_t align) override final
		{
			auto p = mUpperStream->allocate(bytes, align);
			print(p, bytes, align, "alloc");
			return p;
		}

		virtual void do_deallocate(void*p, std::size_t bytes, std::size_t align) override final
		{
			print(p, bytes, align, "~alloc");
			mUpperStream->deallocate(p, bytes, align);
		}

		virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override final
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