#include <iostream>
#include <vector>
#include <iostream>
#include <string>

#include "factorial.hpp"
#include "fibonacci.hpp"
#include "itoa.hpp"
#include "itoax.hpp"

int main()
{
	std::cout << KB::factorial<6>::value << std::endl << std::endl;

	std::cout << KB::fibonacci<5>::value << std::endl << std::endl;

	std::cout << KB::to_string<15> << std::endl;
	std::cout << itoax<15>::value << std::endl << std::endl;

	std::cout << KB::to_string<35, 2> << std::endl;
	std::cout << itoax<35, 2>::value << std::endl;

	return 0;
}