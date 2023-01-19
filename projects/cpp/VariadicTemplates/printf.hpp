#pragma once

#include <iostream>

namespace KB
{
	// source : https://en.cppreference.com/w/cpp/language/parameter_pack

    void printf(const char* format)
    {
        std::cout << format;
    }

    template<typename T, typename... Targs>
    void printf(const char* format, T value, Targs... Fargs)
    {
        for (; *format != '\0'; format++)
        {
            if (*format == '%')
            {
                std::cout << value;
                printf(format + 2, Fargs...);
                return;
            }
            std::cout << *format;
        }
    }
}