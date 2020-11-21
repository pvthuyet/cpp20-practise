module;

#include <iostream>

export module Fibo.AddressHelper;

export namespace fibo::AddressHelper
{
	template<typename T>
	void print(T const& t, std::string_view name = "", int indents = 0)
	{
		for (int i = 0; i < indents; ++i) std::cout << ' ';
		if (not name.empty()) std::cout << name << " ";
		std::cout << &t << std::endl;
	}

	template<typename T, typename U>
	long long diff(T& t, U& u)
	{
		return (long long)(&t) - (long long)(&u);
	}
}