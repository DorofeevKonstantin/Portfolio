#pragma once
namespace KB
{
	// source : https://eli.thegreenplace.net/2014/variadic-templates-in-c/

	template<typename T>
	bool pair_comparer(T leftObj, T rightObj)
	{
		return leftObj == rightObj;
	}

	template<typename T, typename... Args>
	bool pair_comparer(T leftObj, T rightObj, Args... args)
	{
		return leftObj == rightObj && pair_comparer(args...);
	}
}