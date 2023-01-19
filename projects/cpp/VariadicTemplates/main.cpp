#include <iostream>
#include <typeinfo>
#include <string>
#include <utility>

#include "pairComparer.hpp"
#include "printf.hpp"
#include "tuple.hpp"

template<typename... Args>
void foo(Args... args)
{
	std::cout << "foo get " << sizeof...(args) << " args" << std::endl << std::endl;
	//std::cout << args[0] << args[1] << args[2] << std::endl; // can't get
}

void uncomfortableTupleUsing()
{
	std::cout << "uncomfortableTupleUsing : " << std::endl;
	KB::tuple<int, double, std::string> myTuple(12, 2.34, "hello");
	std::cout << typeid(myTuple.head).name() << ":" << myTuple.head << std::endl
		<< typeid(myTuple.next.head).name() << ":" << myTuple.next.head << std::endl
		<< typeid(myTuple.next.next.head).name() << ":" << myTuple.next.next.head << std::endl << std::endl;
		//<< typeid(myTuple.next.next.next.head).name() << ":" << myTuple.next.next.next.head << std::endl;
	system("pause");
}

void comfortableTupleUsing()
{
	std::cout << "comfortableTupleUsing : " << std::endl;
	using triple = KB::tuple<int, double, std::string>;
	triple myTuple(12, 2.34, "hello");
	std::cout << "can call getter<0,...>::get()" << std::endl
		<< KB::getter<0, int, double, std::string>::get(myTuple);

	std::cout << "ideally call get<pos>()" << std::endl
		<< typeid(KB::get<0>(myTuple)).name() << ":" << KB::get<0>(myTuple) << std::endl
		<< typeid(KB::get<1>(myTuple)).name() << ":" << KB::get<1>(myTuple) << std::endl
		<< typeid(KB::get<2>(myTuple)).name() << ":" << KB::get<2>(myTuple) << std::endl << std::endl;
	system("pause");
}

void pairCompares()
{
	std::cout << "pairCompares : " << std::endl;
	//std::cout << "compare(5, 5, 6, 6, 9, 9, 1) == " << KB::pair_comparer(5, 5, 6, 6, 9, 9, 1) << std::endl;
	std::cout << "compare(5, 5, 6, 6, 9, 9) == " << KB::pair_comparer(5, 5, 6, 6, 9, 9) << std::endl;
	std::cout << "compare(\"abc\", \"abc\", \"123\", \"123\") == " << KB::pair_comparer("abc", "abc", "123", "123") << std::endl;
	std::cout << "compare(\"abc\", \"abc\", \"12\", \"123\") == " << KB::pair_comparer("abc", "abc", "12", "123") << std::endl << std::endl;
	system("pause");
}

int main()
{
	foo(1, "abc", 3.14);
	pairCompares();
	KB::printf("KB::printf example:\n%d %f %c %s\n\n", 9, 3.14, 'H', "Hello, world!");
	uncomfortableTupleUsing();
	comfortableTupleUsing();
	return 0;
}