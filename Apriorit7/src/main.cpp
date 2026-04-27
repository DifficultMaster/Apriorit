#include <iostream>
#include <initializer_list>
#include <utility>

#include "apriorit7/core/linked_list.hpp"

int main()
{
    auto list_equals = [](const apriorit7::core::LinkedList<int>& list, std::initializer_list<int> expected)
	{
		auto it = list.begin();
		auto expected_it = expected.begin();

		for (; it != list.end() && expected_it != expected.end(); ++it, ++expected_it)
		{
			if (*it != *expected_it)
			{
				return false;
			}
		}

		return it == list.end() && expected_it == expected.end();
	};

	auto print_check = [](const char* name, bool passed)
	{
		std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << '\n';
	};

	apriorit7::core::LinkedList<int> list;
	print_check("default list is empty", list.empty());

	list.push_front(10);
	print_check("push_front makes list non-empty", !list.empty());

	list.pop_front();
	print_check("pop_front removes single element", list.empty());

	list.pop_front();
	print_check("pop_front on empty is safe", list.empty());

	apriorit7::core::LinkedList<int> source;
	source.push_front(1);
    print_check("source has one item", !source.empty());

	apriorit7::core::LinkedList<int> moved(std::move(source));
	print_check("move constructor transfers data", !moved.empty());

	apriorit7::core::LinkedList<int> assigned;
	assigned = std::move(moved);
	print_check("move assignment transfers data", !assigned.empty());

	assigned.clear();
	print_check("clear empties list", assigned.empty());

	apriorit7::core::LinkedList<int> split_source{ 1, 2, 3, 4 };
	apriorit7::core::LinkedList<int> split_tail = split_source.split_when([](const int& value)
	{
		return value == 3;
	});
	print_check("split_when splits at first matching node", list_equals(split_source, { 1, 2 }) && list_equals(split_tail, { 3, 4 }));

	return 0;
}
