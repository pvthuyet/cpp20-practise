module;

#include <iostream>

module import_cycle_foo;

import import_cycle_bar;

namespace cycle
{
	void foo::printAll(bar* bptr)
	{
		std::cout << "foo::printAll:\n";
		//this->print();
		//bptr->print();
	}
}