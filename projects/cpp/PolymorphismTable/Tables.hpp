#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <utility>
#include <string>

// TODO : data_t requires swappable, idAble

template <class data_t>
class ITable
{
public:
	virtual bool add(data_t*) = 0; // requires swap for data_t
	virtual bool del(data_t*) = 0;
	virtual size_t size() const = 0;
	virtual void print() const = 0;
	virtual data_t* find(size_t) = 0; // requires id field in data_t
	virtual ~ITable() {};
};

template <class data_t>
class TableStat : public ITable<data_t>
{
public:
	bool add(data_t* obj) override
	{
		using std::swap;
		if (currentSize >= shortTableMaxSize)
			return false;
		swap(shortTable[currentSize], *obj);
		++currentSize;
		return true;
	}
	bool del(data_t* record) override
	{
		using std::swap;
		if (currentSize == 0)
			return false;
		size_t position = record - shortTable;
		if (position >= currentSize)
			return false;
		for (size_t i = position; i < currentSize - 1; ++i)
			swap(shortTable[i], shortTable[i + 1]);
		--currentSize;
		return true;
	}
	size_t size() const override { return currentSize; }
	void print() const override
	{
		std::cout << "TableStat (" << currentSize << ") print : " << std::endl;
		for (size_t i = 0; i < currentSize; i++)
			std::cout << shortTable[i];
		std::cout << std::endl;
	}
	data_t* find(size_t _id) override
	{
		data_t* result = nullptr;
		for (size_t i = 0; i < currentSize; i++)
		{
			if (shortTable[i].id == _id)
			{
				result = &(shortTable[i]);
				break;
			}
		}
		return result;
	}
	TableStat()
	{
		std::cout << "TableStat()" << std::endl;
	}
	~TableStat() override
	{
		std::cout << "~TableStat()" << std::endl;
	}
private:
	inline static const size_t shortTableMaxSize = 32;
	size_t currentSize = 0;
	data_t shortTable[shortTableMaxSize];
};

template <class data_t>
class TableDyn : public ITable<data_t>
{
public:
	bool add(data_t* record) override
	{
		longTable.push_back(std::make_shared<data_t>(*record));
		return true;
	}
	bool del(data_t* record) override
	{
		longTable.erase(std::remove_if(longTable.begin(), longTable.end(), [&record](const auto& obj)
			{
				return obj->id == record->id;
			}), longTable.end());
		return true;
	}
	size_t size() const override { return longTable.size(); }
	void print() const override
	{
		std::cout << "TableDyn (" << size() << ") print : " << std::endl;
		for (auto elem : longTable)
			std::cout << *elem;
		std::cout << std::endl;
	}
	data_t* find(size_t _id) override
	{
		auto result = std::find_if(longTable.begin(), longTable.end(), [&_id](const auto& obj)
			{
				return _id == obj->id;
			});
		if (result != longTable.end())
			return (*result).get();
		else
			return nullptr;
	}
	TableDyn() { std::cout << "TableDyn()" << std::endl; longTable.reserve(100); }
	~TableDyn() override { std::cout << "~TableDyn()" << std::endl; }
private:
	std::vector<std::shared_ptr<data_t>> longTable;
};

//class ISort