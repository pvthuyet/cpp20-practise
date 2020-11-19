#include <memory_resource>
#include <string>
#include <iostream>

export module Fibo.MemoryTracker;

namespace fibo
{
	export class MemoryTracker : public std::pmr::memory_resource
	{
	public:
		MemoryTracker() = default;
		explicit MemoryTracker(std::pmr::string const& prefix, 
			std::pmr::memory_resource* upper = std::pmr::get_default_resource()) :
			mPrefix{prefix},
			mUpperStream{upper}
		{}

	private:
		virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override
		{
			std::cout << mPrefix << " [allocate] " << bytes << " bytes. Alignment: " << alignment << '\n';
			return mUpperStream->allocate(bytes, alignment);
		}

		virtual void do_deallocate(void*p, std::size_t bytes, std::size_t alignment) override
		{
			std::cout << mPrefix << " [deallocate] " << p << ", " << bytes << " bytes. Alignment: " << alignment << '\n';
			mUpperStream->deallocate(p, bytes, alignment);
		}

		virtual bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override
		{
			if (this == &other) {
				return true;
			}
			return false;
		}

	private:
		std::pmr::string mPrefix;
		std::pmr::memory_resource* mUpperStream{};
	};
}