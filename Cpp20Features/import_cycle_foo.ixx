module;

#include <iostream>

export module import_cycle_foo;

namespace cycle
{
	class bar;
	export class foo
	{
	public:
		void printAll(bar* bptr);

		//void print() const
		//{
		//	std::cout << "foo id: " << mId << std::endl;
		//}

	private:
		int mId{};
	};
}