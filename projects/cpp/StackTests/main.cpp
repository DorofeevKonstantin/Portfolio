#include <iostream>
#include <stack>
#include <deque>
#include <vector>
#include <list>
#include <new>
#include <string>
#include <cstdint>
#include <chrono>

// source : http://jsteemann.github.io/downloads/code/containers.cpp

class Stats
{
public:
	static size_t n;
	static size_t bytes;
	static size_t allocations;
	static size_t frees;
	static bool output;
	static std::vector<std::pair<void*, size_t>> sizes;
	static std::chrono::steady_clock::time_point start;
	static std::chrono::steady_clock::time_point end;
};

// definition of static fields
size_t Stats::n = 0;
size_t Stats::bytes = 0;
size_t Stats::allocations = 0;
size_t Stats::frees = 0;
bool Stats::output = false;
std::vector<std::pair<void*, size_t>> Stats::sizes;
std::chrono::steady_clock::time_point Stats::start;
std::chrono::steady_clock::time_point Stats::end;

static void* alloc(size_t size)
{
	void* p = std::malloc(size);
	if (Stats::output && p != nullptr)
	{
		Stats::bytes += size;
		Stats::allocations++;
		Stats::sizes.emplace_back(p, size);
	}
	return p;
}

static void dealloc(void* p)
{
	if (p == nullptr)
		return;
	if (Stats::output)
	{
		Stats::frees++;
		size_t size = 0;
		for (auto it = Stats::sizes.begin(); it != Stats::sizes.end(); ++it)
		{
			if ((*it).first == p)
			{
				size = (*it).second; // for debug
				Stats::sizes.erase(it);
				break;
			}
		}
	}
	free(p);
}

static void initTest()
{
	Stats::output = true;
	Stats::allocations = 0;
	Stats::frees = 0;
	Stats::bytes = 0;
	Stats::sizes.clear();
	Stats::start = std::chrono::high_resolution_clock::now();
}

static void finishTest()
{
	Stats::output = false;
	size_t size = 0;
	for (auto it = Stats::sizes.begin(); it != Stats::sizes.end(); ++it)
	{
		size += (*it).second;
	}
	Stats::end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(Stats::end - Stats::start);
	std::cout << "\t=> " << "total : " << Stats::bytes << " bytes, " << Stats::allocations << " malloc(times), " << Stats::frees << " free(times), " << duration.count() << " microsecs" << std::endl;
}


void* operator new(size_t size)
{
	return alloc(size);
}

void* operator new[](size_t size)
{
	return alloc(size);
}

void operator delete(void* p)
{
	dealloc(p);
}

void operator delete[](void* p)
{
	dealloc(p);
}

class Benchmark
{
public:
	// n*sizeof(T)*k+C
	static void stackOnVector()
	{
		std::cout << "stack<vector>\t";
		initTest();
		{
			std::stack <uint64_t, std::vector<uint64_t>> foo;
			for (size_t i = 0; i < Stats::n; ++i)
				foo.push(i);
		}
		finishTest();
	}
	// n*sizeof(T)+(n/chunkSize)*sizeof(void*)+C
	static void stackOnDeque()
	{
		std::cout << "stack<deque>\t";
		initTest();
		{
			std::stack<uint64_t, std::deque<uint64_t>> foo;
			for (size_t i = 0; i < Stats::n; ++i)
				foo.push(i);
		}
		finishTest();
	}
	// n*sizeof(T)+2*n*sizeof(void*)+C
	static void stackOnList()
	{
		std::cout << "stack<list>\t";
		initTest();
		{
			std::stack<uint64_t, std::list<uint64_t>> foo;
			for (size_t i = 0; i < Stats::n; ++i)
				foo.push(i);
		}
		finishTest();
	}
};

int main(int argc, char* argv[]) {
	Stats::n = 100'000;
	std::cout << "Enter n : " << std::endl;
	std::cin >> Stats::n;
	Stats::sizes.reserve(10 * Stats::n);
	std::cout << "n = " << Stats::n << std::endl;
	std::cout << "---------" << std::endl;
	Benchmark::stackOnVector();
	Benchmark::stackOnDeque();
	Benchmark::stackOnList();
	std::cout << std::endl << std::endl;
}