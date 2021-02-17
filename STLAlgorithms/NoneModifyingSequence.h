#pragma once
#include <vector>

class NoneModifyingSequence
{
	enum Operation : int
	{
		all_of,
		any_of,
		none_of,
		ranges_all_of,
		ranges_any_of,
		ranges_none_of,
		end
	};

public:
	static void name();
	void show(unsigned int index) const;

private:
	void all_of_operation() const;
	void any_of_operation() const;
	void none_of_operation() const;

	void ranges_all_of_operation() const;
	void ranges_any_of_operation() const;
	void ranges_none_of_operation() const;

private:
	std::vector<int> generateNumber(int size) const;
};

