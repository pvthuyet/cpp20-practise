module;

#include <iostream>

export module import_cycle_bar;

namespace cycle
{
	class foo;
	export class bar
	{
	public:
		void printAll(foo* fptr);

		//void print() const
		//{
		//	std::cout << "bar id: " << mId << std::endl;
		//}

	private:
		int mId{};
	};
}