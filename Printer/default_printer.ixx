#include <iostream>
#include <string>

export module DefaultPrinter;

export struct DefaultPrinter
{
	void printElement(int e)
	{
		std::cout << e << " ";
	}

	void printSeparator()
	{
		std::cout << ", ";
	}

	void printEol()
	{
		std::cout << std::endl;
	}
};