#include "IPCSmartPointer.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/smart_ptr/intrusive_ptr.hpp>
#include <boost/interprocess/smart_ptr/scoped_ptr.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/weak_ptr.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/list.hpp>

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

namespace scp 
{
	class my_class
	{};

	class my_exception
	{};

	//A functor that destroys the shared memory object
	template<class T>
	class my_deleter 
	{
	private:
		using segment_manager = managed_shared_memory::segment_manager;
		segment_manager* mp_segment_manager;

	public:
		using pointer = T*;
		my_deleter(segment_manager* s_mngr) :
			mp_segment_manager{s_mngr}
		{}

		void operator()(pointer object_to_delete)
		{
			BOOST_ASSERT_MSG(nullptr != mp_segment_manager, "Segment manager is null");
			mp_segment_manager->destroy_ptr(object_to_delete);
		}
	};
}
int IPCSmartPointer::scopedPointer()
{
	using namespace scp;
	//Create shared memory
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;

	managed_shared_memory shmem(create_only, "MySharedMemory", 10000);

	for (int i = 0; i < 2; ++i) {
		scp::my_class* my_object = shmem.construct<scp::my_class>("my_object")();
		my_class* my_object2 = shmem.construct<my_class>(anonymous_instance)();
		shmem.destroy_ptr(my_object2);

		my_deleter<my_class> d(shmem.get_segment_manager());
		try {
			scoped_ptr<my_class, my_deleter<my_class>> s_ptr(my_object, d);
			if (i == 1) {
				throw my_exception();
			}
			s_ptr.release();
		}
		catch (const my_exception&) {

		}

		if (0 == i) {
			if (!shmem.find<my_class>("my_object").first) {
				return 1;
			}

			shmem.destroy<my_class>("my_object");
		}
		else {
			//Make sure the object has been deleted
			if (shmem.find<my_class>("my_object").first) {
				return 1;
			}
		}
	}
	return 0;
}

namespace SharedOnly
{
	class MyType {};
	using segment_manager_type = managed_shared_memory::segment_manager;
	using void_allocator_type = allocator<void, segment_manager_type>;
	using deleter_type = deleter<MyType, segment_manager_type>;
	using my_shared_ptr = shared_ptr<MyType, void_allocator_type, deleter_type>;
}

int IPCSmartPointer::sharedPointer()
{
	using namespace SharedOnly;
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;

	managed_shared_memory segment(create_only, "MySharedMemory", 4096);
	my_shared_ptr &shInstance = *segment.construct<my_shared_ptr>("shared ptr")
		(segment.construct<MyType>("object to shared")()
			, void_allocator_type(segment.get_segment_manager())
			, deleter_type(segment.get_segment_manager())
			);
	BOOST_ASSERT(shInstance.use_count() == 1);
	segment.destroy_ptr(&shInstance);
	return 0;
}


namespace SharedAndWeak
{
	struct type_to_share {};
	using shared_ptr_type = managed_shared_ptr<type_to_share, managed_mapped_file>::type;
	using weak_ptr_type = managed_weak_ptr<type_to_share, managed_mapped_file>::type;

	struct shared_ptr_owner
	{
		shared_ptr_owner(const shared_ptr_type &sharedType) :
			shared_ptr_(sharedType)
		{}
		shared_ptr_owner(const shared_ptr_owner& other) :
			shared_ptr_(other.shared_ptr_)
		{}

		shared_ptr_type shared_ptr_;
	};
}

int IPCSmartPointer::sharedWeakPointer()
{
	using namespace SharedAndWeak;
	const char* MappedFile = "MyMappedFile";
	struct file_remove
	{
		file_remove(const char *MappedFile) :
			MappedFile_{ MappedFile }
		{
			file_mapping::remove(MappedFile_);
		}
		~file_remove()
		{
			file_mapping::remove(MappedFile_);
		}
		const char* MappedFile_{};
	} remover(MappedFile);
	
	{
		managed_mapped_file file(create_only, MappedFile, 65536);
		shared_ptr_type local_shared_ptr = make_managed_shared_ptr(
			file.construct<type_to_share>("object to shared")(), file);
		BOOST_ASSERT(local_shared_ptr.use_count() == 1);

		//Share ownership of the object between local_shared_ptr and a new "owner1"
		shared_ptr_owner* owner1 = file.construct<shared_ptr_owner>("owner1")(local_shared_ptr);
		BOOST_ASSERT(local_shared_ptr.use_count() == 2);

		//local_shared_ptr releases object ownership
		local_shared_ptr.reset();
		BOOST_ASSERT(local_shared_ptr.use_count() == 0);
		BOOST_ASSERT(owner1->shared_ptr_.use_count() == 1);

		//Share ownership of the object between "owner1" and a new "owner2"
		auto owner2 = file.construct<shared_ptr_owner>("owner2")(*owner1);
		BOOST_ASSERT(owner1->shared_ptr_.use_count() == 2);
		BOOST_ASSERT(owner2->shared_ptr_.use_count() == 2);
		BOOST_ASSERT(owner1->shared_ptr_.get() == owner2->shared_ptr_.get());
	}//The mapped file is unmapped here. Objects have been flushed to disk
	{
		//Reopen the mapped file and find again all owners
		managed_mapped_file file(open_only, MappedFile);
		shared_ptr_owner *owner1 = file.find<shared_ptr_owner>("owner1").first;
		shared_ptr_owner* owner2 = file.find<shared_ptr_owner>("owner2").first;
		BOOST_ASSERT(owner1 && owner2);

		//Check everything is as expected
		BOOST_ASSERT(file.find<type_to_share>("object to shared").first != 0);
		BOOST_ASSERT(owner1->shared_ptr_.use_count() == 2);
		BOOST_ASSERT(owner2->shared_ptr_.use_count() == 2);
		BOOST_ASSERT(owner1->shared_ptr_.get() == owner2->shared_ptr_.get());

		//Now destroy one of the owners, the reference count drops.
		file.destroy_ptr(owner1);
		BOOST_ASSERT(owner2->shared_ptr_.use_count() == 1);

		//Create a weak pointer
		weak_ptr_type local_observer1(owner2->shared_ptr_);
		BOOST_ASSERT(local_observer1.use_count() == owner2->shared_ptr_.use_count());
		{
			//Create a local shared pointer from the weak pointer
			auto local_shared_ptr = local_observer1.lock();
			BOOST_ASSERT(local_observer1.use_count() == owner2->shared_ptr_.use_count());
			BOOST_ASSERT(local_observer1.use_count() == 2);
		}

		//Now destroy the remaining owner. "object to share" will be destroyed
		file.destroy_ptr(owner2);
		BOOST_ASSERT(file.find<type_to_share>("object to share").first == 0);

		// Test observer
		BOOST_ASSERT(local_observer1.expired());
		BOOST_ASSERT(local_observer1.use_count() == 0);


		//The reference count will be deallocated when all weak pointers
		//disappear. After that, the file is unmapped.
	}

	return 0;
}

namespace UniquePtr
{
	struct MyType
	{
		MyType(int number = 0) : number_(number)
		{}
		int number_;
	};

	using unique_ptr_type = managed_unique_ptr<MyType, managed_mapped_file>::type;
	using unique_ptr_vector_t = vector <unique_ptr_type, allocator<unique_ptr_type, managed_mapped_file::segment_manager>>;
	using unique_ptr_list_t = list<unique_ptr_type, allocator<unique_ptr_type, managed_mapped_file::segment_manager>>;
}

int IPCSmartPointer::uniquePointer()
{
	using namespace UniquePtr;
	const char* MappedFile = "MyMappedFile";

	//Destroy any previous file with the name to be used.
	struct file_remove
	{
		file_remove(const char* MappedFile)
			: MappedFile_(MappedFile) {
			file_mapping::remove(MappedFile_);
		}
		~file_remove() { file_mapping::remove(MappedFile_); }
		const char* MappedFile_;
	} remover(MappedFile);

	{
		managed_mapped_file file(create_only, MappedFile, 65536);

		unique_ptr_type local_unique_ptr = make_managed_unique_ptr(
			file.construct<MyType>("unique object")(), file);
		BOOST_ASSERT(local_unique_ptr.get() != nullptr);

		//Reset the unique pointer. The object is automatically destroyed
		local_unique_ptr.reset();
		BOOST_ASSERT(file.find<MyType>("unique object").first == nullptr);

		//Now create a vector of unique pointers
		auto unique_vector = file.construct<unique_ptr_vector_t>("unique vector")(file.get_segment_manager());

		// Speed optimization
		unique_vector->reserve(100);

		//Now insert all values
		for (int i = 0; i < 100; ++i) {
			unique_ptr_type p = make_managed_unique_ptr(file.construct<MyType>(anonymous_instance)(i), file);
			unique_vector->push_back(boost::move(p));
			BOOST_ASSERT(unique_vector->back()->number_ == i);
		}

		//Now create a list of unique pointers
		auto uinque_list = file.construct<unique_ptr_list_t>("unique list")(file.get_segment_manager());

		//Pass ownership of all values to the list
		for (int i = 99; !unique_vector->empty(); --i) {
			uinque_list->push_front(boost::move(unique_vector->back()));
			//The unique ptr of the vector is now empty...
			BOOST_ASSERT(unique_vector->back() == 0);
			unique_vector->pop_back();
			//...and the list has taken ownership of the value
			BOOST_ASSERT(uinque_list->front() != 0);
			BOOST_ASSERT(uinque_list->front()->number_ == i);
		}
		BOOST_ASSERT(uinque_list->size() != 0);

		//Now destroy the empty vector.
		file.destroy_ptr(unique_vector);
		//The mapped file is unmapped here. Objects have been flushed to disk
	}
	{
		//Reopen the mapped file and find again the list
		managed_mapped_file file(open_only, MappedFile);
		auto unique_list = file.find<unique_ptr_list_t>("unique list").first;
		BOOST_ASSERT(unique_list);
		BOOST_ASSERT(unique_list->size() == 100);

		auto list_it = unique_list->cbegin();
		for (int i = 0; i < 100; ++i, ++list_it) {
			BOOST_ASSERT((*list_it)->number_ == i);
		}

		//Now destroy the list. All elements will be automatically deallocated.
		file.destroy_ptr(unique_list);
	}
	return 0;
}