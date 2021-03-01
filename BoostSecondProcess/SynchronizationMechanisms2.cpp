#include "SynchronizationMechanisms2.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include "SharedMemoryLog.h"

int SynchronizationMechanisms2::start(int argc, char* argv[])
{
	//return Anonymous_mutex_example(argc, argv);
	//return Named_mutex_example(argc, argv);
	return Anonymous_condition_example(argc, argv);
}

int SynchronizationMechanisms2::Anonymous_mutex_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	try {
		//Remove shared memory on destruction
		struct shm_remove
		{
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		//Create a shared memory object.
		shared_memory_object shm
			(open_only               //only create
			, "MySharedMemory"       //name
			, read_write   //read-write mode
		);

		//Map the whole shared memory in this process
		mapped_region region
			(shm           //What to map
			, read_write   //Map it as read-write
			);

		//Get the address of the mapped region
		void* addr = region.get_address();

		//Construct the shared structure in memory
		shared_memory_log* data = static_cast<shared_memory_log*>(addr);

		//Write some logs
		for (int i = 0; i < shared_memory_log::NumItems; ++i) {
			//Lock the mutex
			scoped_lock<interprocess_mutex> lock(data->mutex);
			std::snprintf(data->items[(data->current_line++) % shared_memory_log::NumItems]
				,shared_memory_log::LineSize
				,"%s_%d", "process_b", i);
			std::cout << "process_b " << std::this_thread::get_id() << std::endl;

			if (i == (shared_memory_log::NumItems - 1))
				data->end_b = true;
			//Mutex is released here
		}

		//Wait until the other process ends
		while (1) {
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (data->end_a)
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
int SynchronizationMechanisms2::Named_mutex_example(int argc, char* argv[])
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
			std::stringstream ss;
			//Write to file atomically
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

int SynchronizationMechanisms2::Anonymous_condition_example(int argc, char* argv[])
{
	using namespace boost::interprocess;
	//Create a shared memory object.
	shared_memory_object shm
		(open_only                    //only create
		, "MySharedMemory"              //name
		, read_write                   //read-write mode
	);
	try {
		//Map the whole shared memory in this process
		mapped_region region
			(shm                       //What to map
			, read_write //Map it as read-write
		);

		//Get the address of the mapped region
		void* addr = region.get_address();

		//Obtain a pointer to the shared structure
		trace_queue* data = static_cast<trace_queue*>(addr);

		//Print messages until the other process marks the end
		bool end_loop = false;

		do {
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (!data->message_in) {
				data->cond_empty.wait(lock);
			}
			if (std::strcmp(data->items, "last message") == 0) {
				end_loop = true;
			}
			else {
				//Print the message
				std::cout << data->items << std::endl;
				//Notify the other process that the buffer is empty
				data->message_in = false;
				data->cond_full.notify_one();
			}
		} while (!end_loop);
	}
	catch (interprocess_exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	system("pause");
	return 0;
}