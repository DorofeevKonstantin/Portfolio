#include <iostream>
#include <string>
#include <regex>

void searchExample()
{
	std::string lines[] = { "Roses are #ff0000",
							"violets are #0000ff",
							"all of my base are belong to you" };

	std::regex colorRegex("#([a-f0-9]{2})"
		"([a-f0-9]{2})"
		"([a-f0-9]{2})");

	// simple match
	for (const auto& line : lines)
	{
		std::cout << line << " : " << std::boolalpha
			<< std::regex_search(line, colorRegex) << std::endl;
	}
	std::cout << std::endl;

	// show contents of marked subexpressions within each match
	std::smatch colorMatch;
	for (const auto& line : lines)
	{
		if (std::regex_search(line, colorMatch, colorRegex))
		{
			std::cout << "matches for '" << line << "\'" << std::endl;
			std::cout << "Prefix: '" << colorMatch.prefix() << "\'" << std::endl;
			for (size_t i = 0; i < colorMatch.size(); ++i)
				std::cout << i << ": " << colorMatch[i] << std::endl;
			std::cout << "Suffix: '" << colorMatch.suffix() << "\'" << std::endl << std::endl;
		}
	}

	// repeated search (see also std::regex_iterator)
	std::string log(R"(
        Speed:366
        Mass:	35
        Speed:	378
        Mass:	32
        Speed:	400
		Mass:	30)");
	std::regex r(R"(Speed:\t\d*)");
	std::smatch sm;
	while (regex_search(log, sm, r))
	{
		std::cout << sm.str() << std::endl;
		log = sm.suffix();
	}

	// C-style string demo
	std::cmatch cm;
	if (std::regex_search("this is a test", cm, std::regex("test")))
		std::cout << std::endl << "Found " << cm[0] << " at position " << cm.prefix().length() << std::endl;
}