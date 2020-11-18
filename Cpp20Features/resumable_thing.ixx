#include <coroutine>

export module ResumableThing;

export struct ResumableThing
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type _coro = nullptr;
	explicit ResumableThing(handle_type coro) :
		_coro(coro)
	{}

	~ResumableThing() noexcept
	{
		if (_coro) _coro.destroy();
	}

	void resume() const
	{
		if (_coro) _coro.resume();
	}

	struct promise_type
	{
		ResumableThing get_return_object()
		{
			return ResumableThing(handle_type::from_promise(*this));
		}

		auto initial_suspend() { return std::suspend_never{}; }
		auto final_suspend() noexcept { return std::suspend_never{}; }

		void return_void() {}	// called for a co_return with no argument
								// or falling off the end of a coroutine
		//void return_value(int value) {} // called for a co_return with argument
	};
};