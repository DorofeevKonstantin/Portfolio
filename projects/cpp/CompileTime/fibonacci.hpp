#pragma once

namespace KB
{
	template <int N> struct fibonacci
	{
		static const int value = fibonacci <N - 1 >::value + fibonacci <N - 2 >::value;
	};
	template <> struct fibonacci <0>
	{
		static const int value = 0;
	};
	template <> struct fibonacci <1>
	{
		static const int value = 1;
	};
}