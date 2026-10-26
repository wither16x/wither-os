#ifndef KERNEL_LIB_FMT_H_
#define KERNEL_LIB_FMT_H_

#include "kernel/drivers/serial.h"
#include "kernel/lib/numbers.h"
#include "kernel/lib/types.h"
#include <concepts>
#include <string_view>
#include <type_traits>
#include <string>
#include <utility>

namespace detail
{
    constexpr void reverse_str(char *s)
    {
        for (usize i = 0, j = std::char_traits<char>::length(s)- 1; i < j; i++, j--) {
            char c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }

    template<std::signed_integral T>
    void print_int_to_str(T v, NumericBase base)
    {
        int sign = 0;
        usize i = 0;
        char str[65]{};

        if ((sign = v) < 0)
            v = -v;

        while (v != 0) {
            T rem = v % std::to_underlying(base);
            str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
            v /= std::to_underlying(base);
        }

        if (sign < 0)
            str[i++] = '-';

        reverse_str(str);
        serial::write(str);
    }

    template<std::unsigned_integral T>
    void print_uint_to_str(T v, NumericBase base)
    {
        usize i = 0;
        char str[65]{};

        while (v != 0) {
            T rem = v % static_cast<unsigned int>(std::to_underlying(base));
            str[i++] = static_cast<char>((rem > 9) ? (rem - 10) + 'a' : rem + '0');
            v /= static_cast<unsigned int>(std::to_underlying(base));
        }

        reverse_str(str);
        serial::write(str);
    }

    template<typename T>
    concept FormatArg = std::is_same_v<std::remove_cvref_t<T>, std::string_view>
        or std::is_same_v<std::remove_cvref_t<T>, char>
        or std::signed_integral<std::remove_cvref_t<T>>
        or std::unsigned_integral<std::remove_cvref_t<T>>
        or IsBinaryValue<std::remove_cvref_t<T>>::value
        or IsOctalValue<std::remove_cvref_t<T>>::value
        or IsHexadecimalValue<std::remove_cvref_t<T>>::value;

    template<FormatArg T>
    void write_arg(const T &arg)
    {
        using Ta = std::remove_cvref_t<T>;

        if constexpr (std::is_same_v<Ta, std::string_view>)
            serial::write(arg);
        else if constexpr (std::is_same_v<Ta, char>)
            serial::write(arg);
        else if constexpr (std::signed_integral<Ta>)
            print_int_to_str<Ta>(arg, NumericBase::Decimal);
        else if constexpr (std::unsigned_integral<Ta>)
            print_uint_to_str<Ta>(arg, NumericBase::Decimal);
        else if constexpr (IsBinaryValue<Ta>::value)
            print_uint_to_str<decltype(arg.v)>(arg.v, NumericBase::Binary);
        else if constexpr (IsOctalValue<Ta>::value)
            print_uint_to_str<decltype(arg.v)>(arg.v, NumericBase::Octal);
        else if constexpr (IsHexadecimalValue<Ta>::value)
            print_uint_to_str<decltype(arg.v)>(arg.v, NumericBase::Hexadecimal);
    }
} // namespace detail

template<typename... Args>
void print(std::string_view str, Args &&...args)
{
    usize arg_idx = 0;

    for (usize i = 0; i < str.length(); i++) {
        if (str[i] != '{') {
            serial::write(str[i]);
            continue;
        }

        ++i;
        if (i >= str.length() or str[i] != '}') {
            serial::write('{');
            if (i < str.length())
                serial::write(str[i]);
            continue;
        }

        usize k = 0;
        auto write_if_arg = [&](const auto &a) {
            if (k++ == arg_idx)
                detail::write_arg(a);
        };
        (write_if_arg(args), ...);
        ++arg_idx;
    }
}

#endif // KERNEL_LIB_FMT_H_