#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

int inc(int value_);
int dec(int value_);
int square(int value_);

class IniParser
{
private:
	typedef int(*FnPtr)(int);
	std::map<std::string, FnPtr> functionMap;
	std::vector<std::string> sequence;
	int startedValue = 0;
	int result = 0;
	void addFunction(boost::property_tree::ptree& pt, std::string functionName)
	{
		std::string extractedString = pt.get<std::string>(functionName);
		std::cout << functionName << "==" << extractedString << std::endl;
		sequence.push_back(extractedString);
	}

public:
	IniParser()
	{
		functionMap["inc"] = inc;
		functionMap["dec"] = dec;
		functionMap["square"] = square;
	}
	void parseIniFile(const std::string& fileName);
};