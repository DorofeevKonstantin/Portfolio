#pragma once

#include <type_traits>
#include <string>
#include <cmath>

namespace KB
{
	// source : https://github.com/tcsullivan/constexpr-to-string
	inline constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	template<int N, int base, typename char_type,
		std::enable_if_t<std::is_integral_v<decltype(N)>, int> = 0,
		std::enable_if_t<(base > 1 && base < sizeof(digits)), int> = 0>
		class to_string_t
	{
		char_type buf[([]() constexpr noexcept
			{
				unsigned int len = N > 0 ? 1 : 2;
				for (auto n = N; n; len++, n /= base);
				return len;
			}())] = {};
	public:
		constexpr to_string_t() noexcept
		{
			auto ptr = end();
			*--ptr = '\0';
			if (N != 0)
			{
				for (auto n = N; n; n /= base)
					*--ptr = digits[(N < 0 ? -1 : 1) * (n % base)];
				if (N < 0)
					*--ptr = '-';
			}
			else
				buf[0] = '0';
		}
		
		//constexpr operator char_type* () noexcept { return buf; }
		constexpr operator const char_type* () const noexcept { return buf; }
		constexpr auto size() const noexcept { return sizeof(buf) / sizeof(buf[0]); }
		constexpr auto end() noexcept { return buf + size(); }
		constexpr const auto end() const noexcept { return buf + size(); }
		//constexpr operator const int() const noexcept { return 1; } WHY???
	};

	// for comfortable using with base == 10
	template<int N, int base = 10, typename char_type = char>
	constexpr KB::to_string_t<N, base, char_type> to_string;
}