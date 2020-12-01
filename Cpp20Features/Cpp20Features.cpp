// Cpp20Features.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <vld.h>
#include <iostream>
#include <ranges>
#include <future>
#include <coroutine>
#include <thread>
#include <chrono>
#include <random>
#include <exception>
#include <memory_resource>
#include <queue>
#include <ranges>
#include <algorithm>
#include <map>
#include <numeric>
#include <iterator>
#include <sstream>
#include "printer_concepts.h"

import Fibo.Pmr;
import Fibo.Pmr2;
import Fibo.MemoryTracker;
import range_exam;
import fibo.coro;

using namespace std;

#define LOGINFO	(std::cout << "[" << std::this_thread::get_id() << "] [" << __func__ << "] ")

import sfinae;

int testSfinaeCpp11()
{
	auto sum = sumCpp11(1, 2, 3.7);
	std::cout << sum << std::endl;
	return 0;
}

int testSfinaeCpp17()
{
	auto sum = sumCpp17(1, 2, 3.7f);
	std::cout << sum << std::endl;
	return 0;
}

int testSfinaeCpp17Fold()
{
	auto sum = sumCpp17Fold(1, 2, 3.7);
	std::cout << sum << std::endl;
	return 0;
}

int testSfinaeCpp20()
{
	auto sum = sumCpp20(1.0, 2, 3.7);
	std::cout << sum << std::endl;
	return 0;
}


int main()
{
	std::pmr::set_default_resource(fibo::getTrackerMemoryResource());
	try
	{
		char input;
		while (true) {
			std::cout << "Cpp20 example. Please choose the feature. Press 'h' for help\n";
			std::cin >> input;
			switch (input) {
			case 'h':
			case 'H':
				std::cout << "Command parameters:\n";
				std::cout << "\t'h' or 'H' - help\n";
				std::cout << "\t'1' - test coroutines\n";
				std::cout << "\t'q' - exit\n";
				break;

			case '1':
				{
					do {
						std::cout << "*******************************\n";
						std::cout << "Generate a string by coroutines\n";
						testCpprefGenerator();
						std::cout << "Continue y/n ? ";
						char con;
						std::cin >> con;
						if (con != 'y') {
							break;
						}
					} while (true);
				}
				break;

			case 'q':
			default:
				return 0;
			}
		}

		//testIntGenerator();
		//testStringGenerator();
		//testCpprefGenerator();
		//monotonic_on_stack();
		//monotonic_unreusable();
		//synchronized_reusable();

		//fibo::pmr2::testPmr2();

		// ranges **********************
		//fibo::ranges::exam1();

	}
	catch (std::exception const& ex)
	{
		LOGINFO << ex.what() << '\n';
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
