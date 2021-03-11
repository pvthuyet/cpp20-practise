#include "IPCSmartPointer.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/smart_ptr/intrusive_ptr.hpp>

using namespace boost::interprocess;

namespace N
{
	//A class that has an internal reference count
	class reference_counted_class
	{
	private:
		using segment_manager = managed_shared_memory::segment_manager;
		unsigned int m_use_count{};
		offset_ptr<segment_manager> mp_segment_manager;

	public:
		reference_counted_class(const reference_counted_class&) = delete;
		reference_counted_class& operator=(const reference_counted_class&) = delete;

		reference_counted_class(segment_manager* s_mngr):
			m_use_count{ 0 }, mp_segment_manager{ s_mngr }
		{}
		~reference_counted_class(){}

	public:
		unsigned int use_count() const
		{
			return m_use_count;
		}

		//add a reference
		inline friend void intrusive_ptr_add_ref(reference_counted_class* p)
		{
			++p->m_use_count;
		}

		inline friend void intrusive_ptr_release(reference_counted_class* p)
		{
			if (--p->m_use_count == 0) {
				p->mp_segment_manager->destroy_ptr(p);
			}
		}
	};
}

//A class that has an intrusive pointer to reference_counted_class
class intrusive_ptr_owner
{
	using intrusive_ptr_t = intrusive_ptr<N::reference_counted_class, offset_ptr<void>>;
	intrusive_ptr_t m_intrusive_ptr{};

public:
	intrusive_ptr_owner(N::reference_counted_class *ptr) :
		m_intrusive_ptr{ptr}
	{}
};

int IPCSmartPointer::intrusivePointer()
{
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;

	// Create shared memory
	managed_shared_memory shmem(create_only, "MySharedMemory", 10000);

	//Create the unique reference counted object in shared memory
	auto ref_counted = shmem.construct<N::reference_counted_class>
		("ref_counted")(shmem.get_segment_manager());

	//Create an array of ten intrusive pointer owners in shared memory
	auto intrusive_ower_array = shmem.construct<intrusive_ptr_owner>
		(anonymous_instance)[10](ref_counted);

	//Now test that reference count is ten
	if (ref_counted->use_count() != 10) {
		BOOST_ASSERT_MSG(false, "Wrong construct intrusive reference count");
		return 1;
	}

	//Now destroy the array of intrusive pointer owners
	//This should destroy every intrusive_ptr and because of
	//that reference_counted_class will be destroyed
	shmem.destroy_ptr(intrusive_ower_array);

	//Now the reference counted object should have been destroyed
	if (shmem.find<intrusive_ptr_owner>("ref_counted").first) {
		BOOST_ASSERT_MSG(false, "Wrong destroy intrusive reference count");
		return 1;
	}
	// Success
	return 0;
}
