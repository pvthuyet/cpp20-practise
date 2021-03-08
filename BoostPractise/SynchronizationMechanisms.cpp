#include "SynchronizationMechanisms.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/containers/list.hpp>
#include "SharedMemoryLog.h"

int SynchronizationMechanisms::start(int argc, char* argv[])
{
	//return Anonymous_mutex_example(argc, argv);
	//return Named_mutex_example(argc, argv);
	//return Anonymous_condition_example(argc, argv);
	//return Anonymous_semaphore_example(argc, argv);
	return Using_a_message_queue(argc, argv);
}

int SynchronizationMechanisms::Anonymous_mutex_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	try {
		//Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove() { shared_memory_object::remove("MySharedMemory"); }
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		//Create a shared memory object.
		shared_memory_object shm
		(create_only               //only create
			, "MySharedMemory"          //name
			, read_write   //read-write mode
		);

		//Set size
		shm.truncate(sizeof(shared_memory_log));

		//Map the whole shared memory in this process
		mapped_region region
			(shm           //What to map
			, read_write   //Map it as read-write
			);

		//Get the address of the mapped region
		void* addr = region.get_address();

		//Construct the shared structure in memory
		shared_memory_log* data = new (addr) shared_memory_log;

		//Write some logs
		for (int i = 0; i < shared_memory_log::NumItems; ++i) {
			//Lock the mutex
			scoped_lock<interprocess_mutex> lock(data->mutex);
			std::snprintf(data->items[(data->current_line++) % shared_memory_log::NumItems]
				,shared_memory_log::LineSize
				,"%s_%d", "process_a", i);
			std::cout << "process_a " << std::this_thread::get_id() << std::endl;

			if (i == (shared_memory_log::NumItems - 1))
				data->end_a = true;
			//Mutex is released here
		}

		//Wait until the other process ends
		while (1) {
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (data->end_b)
				break;
		}
	}
	catch (interprocess_exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	system("pause");
	return 0;
}

const char* filePath = "D:\\test\\file_name";
int SynchronizationMechanisms::Named_mutex_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	try {
		struct file_remove
		{
			file_remove() { std::remove(filePath); }
			//~file_remove() { std::remove("file_name"); }
		} file_remover;

		struct mutex_remove
		{
			mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
			~mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
		} remover;

		//Open or create the named mutex
		named_mutex mutex(open_or_create, "fstream_named_mutex");

		std::ofstream file(filePath, std::ios::app);
		if (!file.is_open()) {
			std::cout << "Failed open " << filePath << std::endl;
		}

		for (int i = 0; i < 10; ++i) {

			std::this_thread::sleep_for(std::chrono::seconds(2));

			//Write to file atomically
			std::stringstream ss;
			scoped_lock<named_mutex> lock(mutex);
			ss << "Process name, " << std::this_thread::get_id() << " ";
			ss << "This is iteration #" << i << std::endl;
			file << ss.str();
			file.flush();
			std::cout << ss.str();
		}
	}
	catch (interprocess_exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	system("pause");
	return 0;
}

int SynchronizationMechanisms::Anonymous_condition_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	//Erase previous shared memory and schedule erasure on exit
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;

	//Create a shared memory object.
	shared_memory_object shm
		(create_only               //only create
		, "MySharedMemory"           //name
		, read_write                //read-write mode
	);

	try {
		//Set size
		shm.truncate(sizeof(trace_queue));

		//Map the whole shared memory in this process
		mapped_region region
			(shm                       //What to map
			, read_write //Map it as read-write
		);

		//Get the address of the mapped region
		void* addr = region.get_address();

		//Construct the shared structure in memory
		trace_queue* data = new (addr) trace_queue;

		const int NumMsg = 100;
		for (int i = 0; i < NumMsg; ++i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (data->message_in) {
				data->cond_full.wait(lock);
			}
			if (i == NumMsg - 1) {
				std::snprintf(data->items,
					trace_queue::LineSize,
					"%s",
					"last message"
				);
			}
			else {
				std::snprintf(data->items,
					trace_queue::LineSize,
					"%s_%d",
					"my_trace", i);
			}
			std::cout << data->items << std::endl;

			//Notify to the other process that there is a message
			data->cond_empty.notify_one();

			//Mark message buffer as full
			data->message_in = true;
		}

	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	system("pause");
	return 0;
}

int SynchronizationMechanisms::Anonymous_semaphore_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	} remover;

	//Create a shared memory object.
	shared_memory_object shm
		(create_only                  //only create
		, "MySharedMemory"              //name
		, read_write  //read-write mode
	);

	//Set size
	shm.truncate(sizeof(shared_memory_buffer));

	//Map the whole shared memory in this process
	mapped_region region
		(shm                       //What to map
		, read_write //Map it as read-write
	);

	//Get the address of the mapped region
	void* addr = region.get_address();

	//Construct the shared structure in memory
	shared_memory_buffer* data = new (addr) shared_memory_buffer;

	const int NumMsg = 100;
	//Insert data in the array
	for (int i = 0; i < NumMsg; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		data->nempty.wait();
		data->mutex.wait();
		data->items[i % shared_memory_buffer::NumItems] = i;
		std::cout << i << " ";
		data->mutex.post();
		data->nstored.post();
	}
	system("pause");
	return 0;
}

int SynchronizationMechanisms::Using_a_message_queue(int argc, char* argv[])
{
	using namespace boost::interprocess;
	try {
		//Erase previous message queue
		message_queue::remove("message_queue");

		//Create a message_queue.
		message_queue mq
			(create_only               //only create
			, "message_queue"           //name
			, 100                       //max message number
			, sizeof(int)               //max message size
		);

		//Send 100 numbers
		for (int i = 0; i < 100; ++i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			mq.send(&i, sizeof(i), 0);
			std::cout << "sent: " << i << "\n";
		}
	}
	catch (interprocess_exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	system("pause");
	return 0;
}

int SynchronizationMechanisms::use_of_managed_external_buffer(int argc, char* argv[])
{
	using namespace boost::interprocess;

	//Create the static memory who will store all objects
	const int memsize = 65536;
	static boost::aligned_storage<memsize>::type static_buffer;

	//This managed memory will construct objects associated with
	//a wide string in the static buffer
	wmanaged_external_buffer objects_in_static_memory
	(create_only, &static_buffer, memsize);

	//We optimize resources to create 100 named objects in the static buffer
	objects_in_static_memory.reserve_named_objects(100);

	//Alias an integer node allocator type
	//This allocator will allocate memory inside the static buffer
	typedef allocator<int, wmanaged_external_buffer::segment_manager>
		allocator_t;

	//Alias a STL compatible list to be constructed in the static buffer
	typedef list<int, allocator_t>    MyBufferList;

	//The list must be initialized with the allocator
	//All objects created with objects_in_static_memory will
	//be stored in the static_buffer!
	MyBufferList* list = objects_in_static_memory.construct<MyBufferList>(L"MyList")
		(objects_in_static_memory.get_segment_manager());

	//Since the allocation algorithm from wmanaged_external_buffer uses relative
	//pointers and all the pointers constructed int the static memory point
	//to objects in the same segment,  we can create another static buffer
	//from the first one and duplicate all the data.
	static boost::aligned_storage<memsize>::type static_buffer2;
	std::memcpy(&static_buffer2, &static_buffer, memsize);

	//Now open the duplicated managed memory passing the memory as argument
	wmanaged_external_buffer objects_in_static_memory2
	(open_only, &static_buffer2, memsize);

	//Check that "MyList" has been duplicated in the second buffer
	if (!objects_in_static_memory2.find<MyBufferList>(L"MyList").first)
		return 1;

	//Destroy the lists from the static buffers
	objects_in_static_memory.destroy<MyBufferList>(L"MyList");
	objects_in_static_memory2.destroy<MyBufferList>(L"MyList");
	return 0;
}