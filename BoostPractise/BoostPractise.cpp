// BoostPractise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
int main(int argc, char* argv[])
{
	using namespace boost::interprocess;
	if (argc == 1) { // Parent process
		// Remove share memory on construction and destruction
		struct shm_remove {
			shm_remove() { shared_memory_object::remove("MySharedMemory"); }
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		// Create a managed shared memory segment
		managed_shared_memory segment(create_only, "MySharedMemory", 65536);
		managed_shared_memory::size_type free_memory = segment.get_free_memory();
		void* shptr = segment.allocate(1024);

		//Check invariant
		if (free_memory < segment.get_free_memory()) {
			return 1;
		}

		//An handle from the base address can identify any byte of the shared
		//memory segment even if it is mapped in different base addresses
		managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
		std::stringstream ss;
		ss << argv[0] << " " << handle;
		ss << std::ends;

		// 
		// Allocate a portion of the segment (raw memory)

	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file