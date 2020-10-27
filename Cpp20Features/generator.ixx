#include <coroutine>
#include <iterator>
#include <compare>
#include <concepts>
#include <string>

export module Generator;

template<typename T> 
struct Generator
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type _coro = nullptr;

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

		T operator*() const noexcept
		{
			if (_coro) return _coro.promise()._val;
			return T{};
		}

		bool operator==(const iterator& rhs) const noexcept
		{
			return *(*this) == *rhs;
		}
	};

	explicit Generator(handle_type coro) : _coro(coro) {}

	~Generator() noexcept
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

	struct promise_type
	{
		T _val;
		Generator get_return_object()
		{
			return Generator(handle_type::from_promise(*this));
		}

		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() { return std::suspend_always{}; }
		auto yield_value(T const& val)
		{
			_val = val;
			return std::suspend_always{};
		}
		void return_void() {}
		void unhandle_exception() {}
	};
};

export using IntGenerator = Generator<int>;
export using StringGenerator = Generator<std::string>;
