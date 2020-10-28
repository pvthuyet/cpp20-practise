#include <coroutine>
#include <iterator>
#include <compare>
#include <concepts>
#include <string>
#include <chrono>
#include <random>

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
		void unhandled_exception() {}
	};
};
//===================================================================================================================

export using IntGenerator = Generator<int>;
export IntGenerator integers(int first, int last)
{
	for (int i = first; i <= last; ++i)
	{
		co_yield i;
	}
}
//===================================================================================================================

export using StringGenerator = Generator<std::string>;
export StringGenerator genStrings()
{
	constexpr const char alphabet[] =
	{ '0','1','2','3','4','5','6','7','8','9',
	  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<unsigned> dis(0, sizeof(alphabet) - 1);
	constexpr unsigned len = 13;
	std::string s(len, 0);
	for (;;)
	{
		for (unsigned i = 0; i < len; ++i) {
			s[i] = alphabet[dis(gen)];
		}
		co_yield s;
	}
}