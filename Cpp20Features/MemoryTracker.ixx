module;

#include <memory_resource>
#include <string>
#include <iostream>
#include <atomic>

export module Fibo.MemoryTracker;

namespace fibo
{
	class MemoryTracker : public std::pmr::memory_resource
	{
		void print(void* p, std::size_t bytes, std::size_t alignment, std::string_view msg)
		{
			static std::atomic_size_t totalMem = 0;
			static std::atomic_int spaces = 0;
			int loop = spaces;
			if (msg.at(0) == '~') {
				loop = --spaces;
				totalMem.fetch_sub(bytes);
			}
			else {
				loop = spaces++;
				totalMem.fetch_add(bytes);
			}

			for (int i = 0; i < loop; ++i) std::cout << "  ";
			std::cout << "[" << msg << "] " 
				<< p << ", " 
				<< bytes << " bytes, align: " << alignment 
				<< "\tmem used: " << totalMem << " bytes"
				<< std::endl << std::flush;
		}

	private:
		virtual void* do_allocate(std::size_t bytes, std::size_t align) override
		{
			auto p = mUpperStream->allocate(bytes, align);
			print(p, bytes, align, "alloc");
			return p;
		}

		virtual void do_deallocate(void*p, std::size_t bytes, std::size_t align) override
		{
			print(p, bytes, align, "~alloc");
			mUpperStream->deallocate(p, bytes, align);
		}

		virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override
		{
			return this == &other;
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