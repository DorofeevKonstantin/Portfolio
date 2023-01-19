#include <string>
#include <iostream>
#include <fstream>
#include <set>

#include "mapExamples.hpp"
#include "IniParser.hpp"

int main()
{
	histogrammExample();
	findInsertEraseExamples();
	multimapExample();
	textAnalyze();

	IniParser parser;
	parser.parseIniFile("options.ini");

	system("pause");
	return 0;
}