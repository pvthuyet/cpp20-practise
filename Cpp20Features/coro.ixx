module;

#include <coroutine>
#include <iostream>

export module fibo.coro;

auto genNum()
{
	int x = 0;
	for (;;) {
		co_yield x++;
	}
}

export void testCoro1()
{
	std::cout << genNum() << std::endl;
}