module;

#include <vector>
#include <memory>
#include <concepts>

export module TickTock2;

namespace fibo
{
	template <typename T>
	concept SizeConstant = std::convertible_to<T, std::size_t> && requires (T t) {
		{ T::value } -> std::convertible_to<std::size_t>;
		std::integral_constant <std::size_t, (std::size_t)T{}>{};
	};

	template<typename T>
	concept Node = std::is_object_v<T>;

	template<typename T>
	concept TreeLocation = requires(T t, const T ct) {
		{ t.isRoot } -> std::convertible_to<bool>;
		t.indices;
		ct.ofChild(0_c);
		requires !T::isRoot || requires {
			{ ct.head() } -> std::convertible_to<std::size_t>;
			ct.tail();
			ct.ofParent();
		};
	};

	template<std::size_t... indices_>
	struct tree_location
	{
		static constexpr const bool isRoot = sizeof...(indices_) == 0;
		std::tuple<std::integral_constant<std::size_t, indices_>...> indices;
		constexpr tree_location() {}
		constexpr tree_location(hana::tuple<hana::size_t<indices_>...>) {}

		auto ofParent() const
		{
			return tree_location{ hana::drop_back(haha::tuple<hana::size_t<indices_>...>{}, 1_c) };
		}

		auto tail() const
		{
			auto tail() const
			{
				return tree_location{ hana::drop_front(hana::tuple<hana::size_t<indices_>...>{}, 1_c) };
			}
		}
	};

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

		}
		void sendUp(const message_base& msg)
		{

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
}