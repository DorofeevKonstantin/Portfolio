#include <iostream>
#include <iterator>
#include <regex>
#include <string>

void replaceExample()
{
	std::string text = "Quick brown fox";
	std::regex vowelRe("a|e|i|o|u");

	// construct a string holding the results
	std::cout << std::endl << std::regex_replace(text, vowelRe, "") << std::endl;
}