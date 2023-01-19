#include <iostream>
#include <list>
#include <stack>

#include "kb_list.hpp"

void binarizationExample(size_t value)
{
	std::stack<int, KB::list<int>> s;
	std::cout << std::endl << value << " as binary : " << std::endl;
	while (value)
	{
		s.push(value % 2);
		value /= 2;
	}
	while (!s.empty())
	{
		std::cout << s.top();
		s.pop();
	}
	std::cout << std::endl;
}

int main()
{
	std::cout << "Test std::stack with KB::list." << std::endl;
	std::stack<int, KB::list<int>> s;
	for (size_t i = 0; i < 10; i++)
		s.push(i);
	while (!s.empty())
	{
		std::cout << s.top() << " ";
		s.pop();
	}
	binarizationExample(15);
	binarizationExample(256);
	binarizationExample(300);
	std::cout << "End of test." << std::endl;
	return 0;
}