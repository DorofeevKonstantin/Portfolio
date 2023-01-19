#include <iostream>

#include "Record.hpp"

bool Record::operator==(const Record* obj)
{
	return this->id == obj->id;
}

void swap(Record& lhs, Record& rhs)
{
	std::swap(lhs.id, rhs.id);
	std::swap(lhs.name, rhs.name);
}

std::ostream& operator<<(std::ostream& os, const Record& obj)
{
	os << obj.id << "," << obj.name << " ";
	return os;
}