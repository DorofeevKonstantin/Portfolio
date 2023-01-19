#pragma once

#include <iostream>
#include <string>

class Record
{
public:
	size_t id;
	std::string name;
	friend std::ostream& operator<<(std::ostream& os, const Record& obj);
	bool operator==(const Record* obj);
};

void swap(Record& lhs, Record& rhs);