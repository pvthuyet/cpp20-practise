#include <coroutine>
#include <optional>

export module CpprefGenerator;

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
			static void unhandled_exception() { throw; }

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
			m_coroutine{ other.m_coroutine }
		{
			other.m_coroutine = {};
		}

		Generator& operator=(Generator&& other) noexcept
		{
			if (this != &other)
			{
				this->~Generator();
				this->m_coroutine = other.m_coroutine;
				other.m_coroutine = {};
			}
			return *this;
		}

		// Range-based for loop support.
		class Iter
		{
		public:
			explicit Iter(Handle coroutine) :
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

		Iter begin()
		{
			if (m_coroutine) m_coroutine.resume();
			return Iter{ m_coroutine };
		}

		std::default_sentinel_t end() { return {}; }

	private:
		Handle m_coroutine = nullptr;
	};
	//===================================================================================================================
}

export namespace cppref
{
	export template<std::integral T>
	Generator<T> range(T first, T last)
	{
		while (first <= last)
		{
			co_yield first++;
		}
	}
}
