#include <coroutine>

export module FibCoroutines;

export struct fib
{
	struct promise_type
	{
		fib get_return_object() { return fib(*this); }
		auto initial_suspend() { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		auto yield_value(int x) { val = x; return std::suspend_always{}; }
		void return_void() {}
		void unhandle_exception() {}
		int val;
	};
	using handle_type = std::coroutine_handle<promise_type>;

	fib(promise_type& p)
	{
		h = handle_type::from_promise(p);
	}

	bool done()
	{
		return h.done();
	}

	int next()
	{
		h.resume();
		return 1;
	}

	handle_type h;
};