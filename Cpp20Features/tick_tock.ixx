module;

#include <memory>
#include <vector>
#include <concepts>

export module TickTock;

namespace fibo
{
	struct message_base
	{
	};

	struct component_base
	{
		virtual void handle(const message_base& msg) = 0;
		std::vector<std::unique_ptr<component_base>> children;
		component_base* parent;
		void sendDown(const message_base& msg)
		{
			for (auto const& item : children) {
				item->handle(msg);
				item->sendDown(msg);
			}
		}
		void sendUp(const message_base& msg)
		{
			if (parent) {
				parent->handle(msg);
				parent->sendUp(msg);
			}
		}
	};

	template<typename... Ms>
	struct component 
	{
	};

	template<typename T, typename... Ts>
	struct component<T, Ts...> : component<Ts...>
	{
		virtual void handle(const T& msg) = 0;

		template<typename M>
		void sendDown(const M& msg)
		{
		}

		template<typename M>
		void sendUp(const M& msg)
		{
		}
	};

	struct start {};
	struct tick {};
	struct tock {};

	struct tick_tock : component<start, tock>
	{
		void handle(const start& msg) final
		{
			puts("tick");
			sendDown(tick{});
		}

		void handle(const tock& msg) final
		{
			puts("tock");
		}
	};

	struct responder : component<tick>
	{
		void handle(const tick& msg) final
		{
			sendUp(tock{});
		}
	};

	tick_tock tok;
}