#include "mapExamples.hpp"

void histogrammExample()
{
	int i = 0, rand;
	std::mt19937 mt_rand((unsigned int)time(0));
	std::map<int, int> histogramm;
	std::cout << "Generating 1 000 000 random numbers : " << std::endl;
	while (i < 1'000'000)
	{
		rand = mt_rand() % 10;
		for (int j = 2; j < 9; ++j)
		{
			if (rand % j == 0)
				histogramm[j]++;
			i++;
		}		
	}
	for (auto& entry : histogramm)
	{
		std::cout << entry.first << " : " << entry.second << std::endl;
	}
	std::cout << std::endl;
	system("pause");
}

void outputMap(std::map<std::string, std::string>& phones)
{
	for (auto& entry : phones)
	{
		std::cout << entry.first << "\t : \t" << entry.second << std::endl;
	}
}

void findInsertEraseExamples()
{
	setlocale(0, "Ru");
	std::map<std::string, std::string> phones;
	phones.insert(std::make_pair("Ivan", "89047843231"));
	std::ifstream in("input.txt");
	std::pair<std::string, std::string> entry;
	while (in >> entry.first >> entry.second)
	{
		phones.insert(entry);
	}
	phones["Петр"] = "89005437721";
	outputMap(phones);

	// search by key
	auto it = phones.find("Ivan");
	if (it != phones.end())
		std::cout << "Ivan finded  ->  " << it->first << " " << it->second << std::endl;
	else
		std::cout << "Ivan not exist" << std::endl;

	// search by value
	std::string value = "89005437721";
	for (auto& entry : phones) // O(N)
	{
		if (entry.second == value)
		{
			std::cout << "Finded value " << value << " -> " << entry.first << " " << entry.second << std::endl;
		}
	}

	// erase by key
	phones.erase("Ivan");

	// erase
	it = phones.find("Петр");
	if (it != phones.end())
		phones.erase(it);
	outputMap(phones);
	system("pause");
}

std::map<std::string, int>::iterator findMaxValue(std::map<std::string, int>& text)
{
	std::map<std::string, int>::iterator it = text.begin();
	int max = -1;
	for (auto entry = text.begin(); entry != text.end(); ++entry)
	{
		if (entry->second > max)
		{
			it = entry;
			max = entry->second;
		}
	}
	return it;
}

void multimapExample()
{
	setlocale(0, "Ru");
	std::multimap<char, int> symbols;
	symbols.insert(std::pair<char, int>('a', 10));
	symbols.insert(std::pair<char, int>('b', 20));
	symbols.insert(std::pair<char, int>('b', 30));
	symbols.insert(std::pair<char, int>('b', 40));
	symbols.insert(std::pair<char, int>('c', 50));
	symbols.insert(std::pair<char, int>('c', 60));
	symbols.insert(std::pair<char, int>('d', 60));
	std::cout << "symbols contains:\n";
	for (char ch = 'a'; ch <= 'd'; ch++)
	{
		//std::pair <std::multimap<char, int>::iterator, std::multimap<char, int>::iterator> ret;
		auto ret = symbols.equal_range(ch);
		std::cout << ch << " =>";
		for (std::multimap<char, int>::iterator it = ret.first; it != ret.second; ++it)
			std::cout << ' ' << it->second;
		std::cout << '\n';
	}
	system("pause");
}

void textAnalyze()
{
	std::map<std::string, int> text;
	std::ifstream in("onegin.txt");
	std::string str;
	while (in >> str)
	{
		text[str]++;
		//text.insert(pair<string,int>(str, 1));
	}
	for (auto& entry : text)
	{
		std::cout << entry.first << " " << entry.second << std::endl;
	}
	auto it = findMaxValue(text);
	std::cout << "Самое частое слово:" << std::endl << it->first << " : " << it->second << " раз" << std::endl;
	system("pause");
}