#include <iostream>
#include <string>
#include <regex>

#include "match.hpp"

void matchExample()
{
	// Simple regular expression matching
	const std::string fileNames[] = { "foo.txt", "bar.txt", "baz.dat", "zoidberg" };
	const std::regex txtRegex("[a-z]+\\.txt");
	std::cout << "txtRegex : " << std::endl;
	for (const auto& fileName : fileNames)
	{
		std::cout << fileName << ": " << std::regex_match(fileName, txtRegex) << std::endl;
	}
	std::cout << std::endl;

	// Extraction of a sub-match
	const std::regex baseRegex("([a-z]+)\\.txt");
	std::smatch baseMatch;
	std::cout << "baseRegex : " << std::endl;
	for (const auto& fileName : fileNames)
	{
		if (std::regex_match(fileName, baseMatch, baseRegex))
		{
			// The first sub_match is the whole string; the next
			// sub_match is the first parenthesized expression.
			if (baseMatch.size() == 2)
			{
				std::ssub_match base_sub_match = baseMatch[1];
				std::string base = base_sub_match.str();
				std::cout << fileName << " has a base of " << base << std::endl;
			}
		}
	}
	std::cout << std::endl;

	// Extraction of several sub-matches
	const std::regex piecesRegex("([a-z]+)\\.([a-z]+)");
	std::smatch piecesMatch;
	std::cout << "piecesRegex : " << std::endl;
	for (const auto& fileName : fileNames)
	{
		if (std::regex_match(fileName, piecesMatch, piecesRegex))
		{
			std::cout << fileName << std::endl;
			for (size_t i = 0; i < piecesMatch.size(); ++i)
			{
				std::ssub_match sub_match = piecesMatch[i];
				std::string piece = sub_match.str();
				std::cout << "  submatch " << i << ": " << piece << std::endl;
			}
		}
	}
	std::cout << std::endl;
}