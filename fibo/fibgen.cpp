#include <coroutine>

module FibGenerator;
import FibCoroutines;

fib genFib(int start, int& len)
{
	int field1 = 1;
	int field2 = 0;
	int idx = 1;
	
	while (idx < start)
	{
		int next = field1 + field2;
		field2 = field1;
		field1 = next;
		++idx;
	}

	while (len > 0)
	{
		co_yield field1;
		int next = field1 + field2;
		field2 = field1;
		field1 = next;
		--len;
	}
	co_return;
}