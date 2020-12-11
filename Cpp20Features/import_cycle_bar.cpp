module;

#include <iostream>

module import_cycle_bar;

import import_cycle_foo;

namespace cycle
{
	void bar::printAll(foo* fptr)
	{
		std::cout << "bar::printAll:\n";
		//this->print();
		//fptr->print();
	}
}