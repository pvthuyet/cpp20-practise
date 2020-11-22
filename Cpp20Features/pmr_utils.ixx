module;

#include <memory_resource>
#include <cstddef>

export module Fibo.PmrUtils;

namespace fibo::PmrUtils
{
	export class polymorphic_allocator_delete
	{
	public:
		polymorphic_allocator_delete(std::pmr::polymorphic_allocator<std::byte> alloc) :
			mAlloc{std::move(alloc)}
		{}
		
		template<typename T>
		void operator()(T* ptr)
		{
			//std::pmr::polymorphic_allocator<T>(mAlloc).destroy(ptr);
			std::pmr::polymorphic_allocator<T>(mAlloc).deallocate(ptr, 1);
		}

	private:
		std::pmr::polymorphic_allocator<std::byte> mAlloc;
	};

	export class default_polymorphic_allocator_delete
	{
	public:
		template<typename T>
		void operator()(T* ptr)
		{
			//std::pmr::polymorphic_allocator<T>(mAlloc).destroy(ptr);
			std::pmr::polymorphic_allocator<T>(std::pmr::get_default_resource()).deallocate(ptr, 1);
		}
	};
}