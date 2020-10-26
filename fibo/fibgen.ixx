#include <coroutine>

export module FibGenerator;
import FibCoroutines;

export fib genFib(int start, int& len);