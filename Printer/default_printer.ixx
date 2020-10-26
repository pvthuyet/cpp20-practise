#include <iostream>
#include <string>

export module DefaultPrinter;

export struct DefaultPrinter
{
	void printMsg(std::string const& s)
	{
		std::cout << s << std::endl;
	}

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