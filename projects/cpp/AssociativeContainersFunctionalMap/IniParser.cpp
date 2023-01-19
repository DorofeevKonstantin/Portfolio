#include "IniParser.hpp"

int inc(int value_)
{
	return ++value_;
}
int dec(int value_)
{
	return --value_;
}
int square(int value_)
{
	return value_ * value_;
}

void IniParser::parseIniFile(const std::string& fileName)
{
	boost::property_tree::ptree pt;
	try
	{
		boost::property_tree::ini_parser::read_ini(fileName, pt);
	}
	catch (const boost::property_tree::ptree_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	addFunction(pt, "f1");
	addFunction(pt, "f2");
	addFunction(pt, "f3");
	startedValue = pt.get<int>("value");

	std::cout << "Started value = " << startedValue << std::endl;
	result = startedValue;
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		result = functionMap[sequence[i]](result);
	}
	std::cout << "Counted value = " << result << std::endl;
}