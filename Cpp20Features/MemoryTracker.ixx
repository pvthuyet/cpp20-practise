module;

#include <memory_resource>
#include <string>
#include <iostream>

export module Fibo.MemoryTracker;

namespace fibo
{
	export class MemoryTracker : public std::pmr::memory_resource
	{
		void print(void* p, std::size_t bytes, std::size_t alignment, std::string_view msg)
		{
			static int spaces = 0;
			auto loop = (msg.at(0) == '~') ? --spaces : spaces++;
			for (int i = 0; i < loop; ++i) std::cout << "  ";
			std::cout << "[" << msg << "] " << p << ", " << bytes << " bytes, align: " << alignment << std::endl << std::flush;
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
}