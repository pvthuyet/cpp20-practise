module;

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

export module sean_parents_object;

namespace saigon
{
	class object_t
	{
	public:
		template<class T>
		object_t(T t) : mSelf{std::make_shared<model<T>>(std::move(t))}
		{}

		friend void draw(object_t const& x, std::ostream& out, size_t position)
		{
			x.mSelf->do_draw(out, position);
		}

	private:
		struct concept_t
		{
			virtual ~concept_t() = default;
			virtual void do_draw(std::ostream&, size_t) const = 0;
		};

		template <class T>
		struct model final : concept_t
		{
			model(T t) : mData(std::move(t)) {}
			void do_draw(std::ostream& out, size_t position) const final
			{ 
				draw(mData, out, position);
			}
			T mData;
		};

		std::shared_ptr<const concept_t> mSelf;
	};

	using document_t = std::vector<object_t>;
	void draw(document_t const& doc, std::ostream& out, size_t position)
	{
		out << std::string(position, ' ') << "<document>" << std::endl;
		for (auto const& e : doc) {
			draw(e, out, position + 2);
		}
		out << std::string(position, ' ') << "</document>" << std::endl;
	}

	void draw(int x, std::ostream& out, size_t position)
	{
		out << std::string(position, ' ') << x << std::endl;
	}

	void draw(std::string s, std::ostream& out, size_t position)
	{
		out << std::string(position, ' ') << s << std::endl;
	}

	class my_class_t {};
	void draw(my_class_t const& x, std::ostream& out, size_t position)
	{
		out << std::string(position, ' ') << "my_class_t" << std::endl;
	}

	export void test_object_t()
	{
		using namespace std::string_literals;
		document_t doc;
		doc.emplace_back(1);
		doc.emplace_back("Hello"s);
		doc.emplace_back(doc);
		doc.emplace_back(my_class_t{});
		std::ranges::reverse(doc);
		draw(doc, std::cout, 0);
	}
}