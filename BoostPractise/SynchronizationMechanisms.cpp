#include "SynchronizationMechanisms.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include "SharedMemoryLog.h"

int SynchronizationMechanisms::start(int argc, char* argv[])
{
	//return Anonymous_mutex_example(argc, argv);
	return Named_mutex_example(argc, argv);
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

int SynchronizationMechanisms::Named_mutex_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	try {
		struct file_remove
		{
			file_remove() { std::remove("file_name"); }
			//~file_remove() { std::remove("file_name"); }
		} file_remover;

		struct mutex_remove
		{
			mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
			~mutex_remove() { named_mutex::remove("fstream_named_mutex"); }
		} remover;

		//Open or create the named mutex
		named_mutex mutex(open_or_create, "fstream_named_mutex");

		std::ofstream file("file_name");
		for (int i = 0; i < 10; ++i) {

			std::this_thread::sleep_for(std::chrono::seconds(2));

			//Write to file atomically
			std::stringstream ss;
			scoped_lock<named_mutex> lock(mutex);
			ss << "Process name, " << std::this_thread::get_id() << " ";
			ss << "This is iteration #" << i << std::endl;
			file << ss.str();
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