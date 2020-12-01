#include <coroutine>
#include <optional>
#include <concepts>
#include <string>
#include <chrono>
#include <random>
#include <stdexcept>
#include <utility>
#include <thread>
#include <iostream>
#include <experimental/generator>
#include <array>

export module CpprefGenerator;

#define LOGINFO	(std::cout << "[" << std::this_thread::get_id() << "] ")

namespace cppref
{
	template<std::movable T>
	class Generator
	{
	public:
		struct promise_type
		{
			Generator<T> get_return_object()
			{
				return Generator{Handle::from_promise(*this)};
			}

			static std::suspend_always initial_suspend() noexcept { return {}; }
			static std::suspend_always final_suspend() noexcept { return {}; }
			std::suspend_always yield_value(T value) noexcept
			{
				current_value = std::move(value);
				return {};
			}
			// Disallow co_await in generator coroutines.
			void await_transform() = delete;
			void return_void() {}
			static void unhandled_exception() 
			{ 
				std::rethrow_exception(std::current_exception());
			}

			std::optional<T> current_value;
		};
		using Handle = std::coroutine_handle<promise_type>;
		
		explicit Generator(Handle coroutine) : 
			m_coroutine{ coroutine } 
		{}

		Generator() = default;
		~Generator() noexcept
		{
			if (m_coroutine) m_coroutine.destroy();
		}

		Generator(Generator const&) = delete;
		Generator& operator=(Generator const&) = delete;

		Generator(Generator&& other) noexcept :
			m_coroutine{ std::exchange(other.m_coroutine, nullptr) }
		{
		}

		Generator& operator=(Generator&& other) noexcept
		{
			if (this != &other)
			{
				this->~Generator();
				this->m_coroutine = std::exchange(other.m_coroutine, nullptr);
			}
			return *this;
		}

		const T& operator()() const
		{
			if (m_coroutine && !m_coroutine.done()) 
			{
				m_coroutine.resume();
			}
			return *m_coroutine.promise().current_value;
		}

		// Range-based for loop support.
		class Iterator
		{
		public:
			explicit Iterator(Handle coroutine) :
				m_coroutine{ coroutine }
			{}

			void operator++() const
			{ 
				m_coroutine.resume();
			}

			const T& operator*() const 
			{ 
				return *m_coroutine.promise().current_value; 
			}

			bool operator==(std::default_sentinel_t) const 
			{ 
				return !m_coroutine || m_coroutine.done(); 
			}

		private:
			Handle m_coroutine;
		};

		Iterator begin()
		{
			if (m_coroutine) m_coroutine.resume();
			return Iterator{ m_coroutine };
		}

		std::default_sentinel_t end() { return {}; }

	private:
		Handle m_coroutine = nullptr;
	};
	//===================================================================================================================


	export template<std::integral T>
	Generator<T> generator_number(T first, T last)
	{
		while (first <= last)
		{
			co_yield first++;
		}
	}

	template<class T>
	concept TString = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

	
	constexpr const std::array<char, 62> alphabet { '0','1','2','3','4','5','6','7','8','9',
	  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

	export Generator<std::string> generator_string(unsigned len)
	{
		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine gen(seed);
		std::uniform_int_distribution<unsigned> dis(0, alphabet.size() - 1);
		std::string s(len, 0);

		for (;;)
		{
			for (unsigned i = 0; i < len; ++i) {
				s[i] = alphabet[dis(gen)];
			}
			LOGINFO << "generator_string generating....\n";
			co_yield s;
		}
		co_return;
	}

	export std::experimental::generator<std::string> generator_string_exper(unsigned len)
	{
		unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine gen(seed);
		std::uniform_int_distribution<unsigned> dis(0, alphabet.size() - 1);
		std::string s(len, 0);

		for (;;)
		{
			for (unsigned i = 0; i < len; ++i) {
				s[i] = alphabet[dis(gen)];
			}
			LOGINFO << "generator_string_exper generating....\n";
			co_yield s;
		}
		co_return;
	}
}
