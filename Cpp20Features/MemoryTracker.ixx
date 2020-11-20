module;

#include <memory_resource>
#include <string>
#include <iostream>

export module Fibo.MemoryTracker;

namespace fibo
{
	export class MemoryTracker : public std::pmr::memory_resource
	{
		void print(void* p, std::size_t bytes, std::size_t alignment, bool alloc = true)
		{
			static int spaces = 0;
			if (alloc) {
				for (int i = 0; i < spaces; ++i) std::cout << ' ';
				std::cout << "[alloc] " << p << ", " << bytes << " bytes, align: " << alignment << '\n';
				spaces++;
			}
			else {
				spaces--;
				for (int i = 0; i < spaces; ++i) std::cout << ' ';
				std::cout << "[~alloc] " << p << ", " << bytes << " bytes, align: " << alignment << '\n';
			}
		}

	private:
		virtual void* do_allocate(std::size_t bytes, std::size_t align) override
		{
			auto p = mUpperStream->allocate(bytes, align);
			print(p, bytes, align);
			return p;
		}

		virtual void do_deallocate(void*p, std::size_t bytes, std::size_t align) override
		{
			print(p, bytes, align, false);
			mUpperStream->deallocate(p, bytes, align);
		}

		virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override
		{
			return this == &other;
		}

	private:
		std::pmr::memory_resource* mUpperStream{ std::pmr::get_default_resource() };
	};
}