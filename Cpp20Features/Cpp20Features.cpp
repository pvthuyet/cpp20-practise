// Cpp20Features.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ranges>
#include <future>
#include <coroutine>
#include <thread>
#include <chrono>
#include "printer_concepts.h"

import FibCoroutines;
import FibGenerator;
import DefaultPrinter;
import BetterPrinter;
import ResumableThing;
import IntGenerator;

using namespace std;

#define LOGINFO	cout << "[" << std::this_thread::get_id() << "] [" << __func__ << "] "

struct DefaultFormat
{
	template<is_series S, is_printer T>
	void format(T t, S s)
	{
		while (!s.done())
		{
			t.printElement(s.next());
			t.printSeparator();
		}
		t.printEol();
	}
};

template<typename S, 
	typename T = DefaultPrinter, 
	typename F = DefaultFormat> 
	requires is_series_printer<T, S, F> void print(S& s)
{
	F{}.format(T{}, s);
}

ResumableThing counter()
{
	LOGINFO << "called\n";
	for (unsigned i = 0;; ++i)
	{
		co_await std::suspend_always{};
		LOGINFO << "resumed (#" << i << ")\n";
	}
}

void testCounter()
{
	using namespace std::chrono_literals;
	LOGINFO << "calling counter\n";
	auto theCounter = counter();
	LOGINFO << "resuming counter\n";
	for (int i = 0; i < 5; ++i) 
	{
		theCounter.resume();
		std::this_thread::sleep_for(1s);
	}
	LOGINFO << "done\n";
}

thread_local int thrVal = 0;
ResumableThing nameCounter(std::string name)
{
	LOGINFO << "called\n";
	for (unsigned i = 0;; ++i)
	{
		co_await std::suspend_always{};
		LOGINFO << name << " resumed (#" << thrVal++ << ")\n";
	}
}

void testNameCounter()
{
	using namespace std::chrono_literals;
	auto counterA = nameCounter("A");
	auto counterB = nameCounter("B");
	counterA.resume();
	counterB.resume();
	std::this_thread::sleep_for(1s);
	counterB.resume();
	counterA.resume();
}

IntGenerator integers(int first, int last)
{
	for (int i = first; i <= last; ++i)
	{
		co_yield i;
	}
}

void testIntGenerator()
{
	for(int x : integers(1, 5))
		LOGINFO << x << '\n';
}

int main()
{
	//testCounter();
	//testNameCounter();
	//testCoroutine();
	testIntGenerator();
	//int len = 6;
	//fib f1 = genFib(1, len);
	//print(f1);
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
