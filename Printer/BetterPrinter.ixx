#include <fmt/color.h>
#include <iostream>

export module BetterPrinter;

export struct Printer
{
	void printElement(int e)
	{
		fmt::print(fg(fmt::color::crimson) |
		fmt::emphasis::bold, "[{}]", e);
	}

	void printSeparator()
	{
		fmt::print(bg(fmt::color::yellow), ", ");
	}

	void printEol()
	{
		std::cout << std::endl;
	}
};