using integer = unsigned long long;

constexpr const char* digits = "0123456789abcdefghij";

template<integer X, integer Base, char... cs>
struct itoa_ : itoa_<X / Base, Base, digits[X % Base], cs...> {};

template<integer Base, char c, char... cs>
struct itoa_<0, Base, c, cs...>
{
	static constexpr const char value[] = { c, cs..., '\0' };
};

// definition of field
//template<integer Base, char c, char... cs>
//constexpr const char itoa_<0, Base, c, cs...>::value[];

// for comfortable using with base == 10
template <integer X, integer Base = 10>
struct itoax : itoa_<X, Base> {};