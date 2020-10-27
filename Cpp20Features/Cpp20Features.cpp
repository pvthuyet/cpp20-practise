// Cpp20Features.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ranges>
#include <future>
#include <coroutine>
#include <thread>
#include <chrono>
#include <random>
#include "printer_concepts.h"

import FibCoroutines;
import FibGenerator;
import DefaultPrinter;
import BetterPrinter;
import ResumableThing;
import Generator;

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
		LOGINFO << "co_yield - integers: " << i << std::endl;
		co_yield i;
	}
}

void testIntGenerator()
{
	//auto theIntegral = integers(1, 5);
	//for(auto it = theIntegral.begin(); it != theIntegral.end(); ++it)
	//	LOGINFO << *it << '\n';

	for (int x : integers(1, 5)) {}
}


StringGenerator genStrings(int num)
{
	constexpr const char alphabet[] =
	{ '0','1','2','3','4','5','6','7','8','9',
	  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
	constexpr unsigned len = 13;
	std::string str(len, 0);

	for (int i = 0; i < num; ++i)
	{
		for (unsigned i = 0; i < len; ++i) {
			str[i] = alphabet[dis(gen)];
		}

		LOGINFO << "co_yield - string: " << str << std::endl;
		co_yield str;
	}
}

void testStringGenerator()
{
	for (auto&& x : genStrings(5)) {
		//LOGINFO << x << '\n';
	}
}

int main()
{
	LOGINFO << "Start\n";
	//testCounter();
	//testNameCounter();
	//testCoroutine();
	//testIntGenerator();
	testStringGenerator();
	//int len = 6;
	//fib f1 = genFib(1, len);
	//print(f1);
	LOGINFO << "End\n";
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
