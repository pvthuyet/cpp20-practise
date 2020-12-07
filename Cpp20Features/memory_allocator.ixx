#include <memory_resource>

export module Saigon.MemoryAllocator;

import Fibo.MemoryTracker;

namespace saigon
{
	export class sso_memory final : public std::pmr::unsynchronized_pool_resource
	{
	protected:
		void* do_allocate(std::size_t bytes, std::size_t align) final;
		void do_deallocate(void* p, std::size_t bytes, std::size_t align) final;
		bool do_is_equal(std::pmr::memory_resource const& other) const noexcept final;
	};

	void* sso_memory::do_allocate(std::size_t bytes, std::size_t align)
	{
		auto p = unsynchronized_pool_resource::do_allocate(16 * bytes, align);
		fibo::print(p, bytes, align, "alloc");
		return p;
	}

	void sso_memory::do_deallocate(void* p, std::size_t bytes, std::size_t align)
	{
		fibo::print(p, bytes, align, "~alloc");
		unsynchronized_pool_resource::do_deallocate(p, bytes, align);
	}

	bool sso_memory::do_is_equal(std::pmr::memory_resource const& other) const noexcept
	{
		return unsynchronized_pool_resource::do_is_equal(other);
	}
}