#pragma once

namespace KB
{
	template <unsigned long long x>
	struct factorial
	{
		static const long long value = x * factorial<x - 1>::value;
	};
	template <> struct factorial<0>
	{
		static const long long value = 1;
	};
}