#include <coroutine>
#include <iterator>
#include <compare>

export module IntGenerator;
export struct IntGenerator
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	struct iterator : std::iterator_traits<std::input_iterator_tag>
	{
		handle_type _coro = nullptr;
		iterator() = default;
		iterator(handle_type coro) : _coro(coro) {}
		iterator& operator++()
		{
			_coro.resume();
			if (_coro.done()) { _coro = nullptr; }
			return *this;
		}

		int operator*() const
		{
			if (_coro) return _coro.promise()._val;
			return 0;
		}

		bool operator==(const iterator& rhs)
		{
			return *(*this) == *rhs;
		}
	};

	handle_type _coro = nullptr;
	
	explicit IntGenerator(handle_type coro) : _coro(coro) {}

	~IntGenerator() noexcept
	{
		if (_coro) _coro.destroy();
	}

	iterator begin()
	{
		if (_coro)
		{
			_coro.resume();
			if (_coro.done()) return end();
		}
		return iterator(_coro);
	}

	iterator end()
	{
		return iterator{};
	}

	void resume() const
	{
		if (_coro) _coro.resume();
	}

	int get()
	{
		if (_coro) return _coro.promise()._val;
	}

	struct promise_type
	{
		int _val;
		IntGenerator get_return_object()
		{
			return IntGenerator(handle_type::from_promise(*this));
		}

		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() { return std::suspend_always{}; }
		auto yield_value(int const& val)
		{
			_val = val;
			return std::suspend_always{};
		}
		void return_void() {}
	};
};