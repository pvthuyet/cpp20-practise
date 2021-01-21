module;

#include <memory>
#include <vector>
#include <concepts>

export module TickTock;

namespace fibo
{
	struct message_base
	{
		virtual void tag() {}
	};

	template<typename M>
	struct handles
	{
		virtual void handle(const M& msg) = 0;
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
	struct component : component_base, handles<Ms>...
	{
		using handles<Ms>::handle...;

		template<typename M>
		bool tryHandle(const message_base& msg)
		{
			if (const M* m = dynamic_cast<const M*>(&msg)) {
				handle(*m);
				return true;
			}
			return false;
		}

		void handle(const message_base& msg) override
		{
			(tryHandle<Ms>(msg) || ...);
		}
	};

	struct start : message_base {};
	struct tick : message_base {};
	struct tock : message_base {};

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

	struct idle : component<>
	{};

	struct responder : component<tick>
	{
		void handle(const tick& msg) final
		{
			sendUp(tock{});
		}
	};

	export void test_ticktock()
	{
		std::unique_ptr<component_base> a = std::make_unique<tick_tock>();
		std::unique_ptr<component_base> b = std::make_unique<idle>();
		std::unique_ptr<component_base> c = std::make_unique<responder>();
		std::unique_ptr<component_base> d = std::make_unique<idle>();
		std::unique_ptr<component_base> e = std::make_unique<responder>();

		e->parent = &*d;
		d->children.push_back(std::move(e));
		d->parent = &*b;
		b->children.push_back(std::move(d));
		c->parent = &*b;
		b->children.push_back(std::move(c));
		b->parent = &*a;
		a->children.push_back(std::move(b));

		a->handle(start{});
	}
}