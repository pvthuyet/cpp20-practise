// Cpp20Features.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ranges>
#include "printer_concepts.h"

import FibCoroutines;
import FibGenerator;
import DefaultPrinter;
import BetterPrinter;

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

int main()
{
	int len = 6;
	fib f1 = genFib(1, len);
	print(f1);
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
