#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <filesystem>

#include "match.hpp"
#include "search.hpp"
#include "replace.hpp"

// sources :
// https://en.wikipedia.org/wiki/Regular_expression
// https://en.cppreference.com/w/cpp/regex
// https://regex101.com/
// https://regexone.com/
// http://regextutorials.com/
// https://www.hackerrank.com/domains/regex

void findPasswordsInTxtFiles()
{
	auto path = std::filesystem::current_path();
	std::vector<std::string> txtFiles;
	for (const auto& file : std::filesystem::recursive_directory_iterator(path))
	{
		if (file.path().extension().compare(".txt") == 0)
			txtFiles.push_back(file.path().string());
	}
	for (const auto& txtFile : txtFiles)
	{
		std::ifstream in(txtFile);
		std::stringstream buffer;
		buffer << in.rdbuf();
		std::string content = buffer.str();
		std::regex passwordRegex("password|pwd|pswd|paswd|pass|passwd");
		auto passwordsBegin = std::sregex_iterator(content.begin(), content.end(), passwordRegex);
		auto passwordsEnd = std::sregex_iterator();
		auto count = std::distance(passwordsBegin, passwordsEnd);
		if (count > 0)
		{
			std::cout << "Found " << count << " possibly passwords in " << txtFile << " : " << std::endl;
			for (std::sregex_iterator i = passwordsBegin; i != passwordsEnd; ++i)
			{
				std::smatch match = *i;
				std::string matchStr = match.suffix();
				std::cout << match.prefix() << " -> " << match.str() << " -> " << match.suffix() << std::endl;
			}
			std::cout << std::endl << std::endl;
		}
	}
}

bool isValidMail(std::string email)
{
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return std::regex_match(email, pattern);
}

void validateExample()
{
	std::string email = "";
	while (!isValidMail(email))
	{
		system("cls");
		std::cout << "Enter your email address : " << std::endl;
		std::cin >> email;
	}
	std::cout << "Accepted." << std::endl;
}

int main()
{
	setlocale(0, "Ru");
	//matchExample();
	//searchExample();
	//replaceExample();
	//findPasswordsInTxtFiles();
	validateExample();
	return 0;
}