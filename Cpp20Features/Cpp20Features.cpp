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
#include "printer_concepts.h"

import CpprefGenerator;
import Fibo.Pmr;
import Fibo.Pmr2;
import Fibo.MemoryTracker;

using namespace std;

#define LOGINFO	cout << "[" << std::this_thread::get_id() << "] [" << __func__ << "] "

void testCpprefGenerator()
{
	auto&& sGen = cppref::range<string>(33);

	int loop = 0;
	while (loop++ < 10)
	{
		LOGINFO << sGen() << std::endl;
		for (auto i : cppref::range(1, 5)) {
			LOGINFO << i << std::endl;
		}
		std::this_thread::sleep_for(1s);
	}
}

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
	//return testSfinae11();
	//return testSfinaeCpp17();
	//return testSfinaeCpp17Fold();
	//return testSfinaeCpp20();

	//LOGINFO << "Start\n";
	try
	{
		//testIntGenerator();
		//testStringGenerator();
		//testCpprefGenerator();
		//monotonic_on_stack();
		//monotonic_unreusable();
		//synchronized_reusable();

		fibo::pmr2::testPmr2();
	}
	catch (std::exception const& ex)
	{
		LOGINFO << ex.what() << '\n';
	}
	//LOGINFO << "End\n";
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
