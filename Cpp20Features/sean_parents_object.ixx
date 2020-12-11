#include <memory>

export module sean_parents_object;

namespace saigon
{
	class object_t
	{
	public:
		template<class T>
		object_t(T t) : mSelf{std::make_shared<model<T>>(std::move(t))}
		{}

		friend void draw(object_t const& x)
		{
			x.mSelf->do_draw();
		}

	private:
		struct concept_t
		{
			virtual ~concept_t() noexcept = default;
			virtual void do_draw() = 0;
		};

		template <class T>
		struct model final : concept_t
		{
			model(T t) : mData{std::move(t)} {}
			~model() noexcept final {}
			void do_draw() final
			{ 
				//draw(mData);
			}
			T mData;
		};

		std::shared_ptr<const concept_t> mSelf;
	};
}