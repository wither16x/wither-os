#ifndef KERNEL_LIB_NUMBERS_H_
#define KERNEL_LIB_NUMBERS_H_

#include <concepts>

enum class NumericBase {
    Binary = 2,
    Octal = 8,
    Decimal = 10,
    Hexadecimal = 16
};

template<std::integral T>
struct BinaryValue {
    T v;
};

template<std::integral T>
struct OctalValue {
    T v;
};

template<std::integral T>
struct HexadecimalValue {
    T v;
};

template<std::integral T>
constexpr BinaryValue<T> bin(T v)
{
    return {v};
}

template<std::integral T>
constexpr OctalValue<T> oct(T v)
{
    return {v};
}

template<std::integral T>
constexpr HexadecimalValue<T> hex(T v)
{
    return {v};
}

template<typename T>
struct IsBinaryValue {
    static constexpr bool value = false;
};

template<typename T>
struct IsBinaryValue<BinaryValue<T>>
{
    static constexpr bool value = true;
};

template<typename T>
struct IsOctalValue {
    static constexpr bool value = false;
};

template<typename T>
struct IsOctalValue<OctalValue<T>>
{
    static constexpr bool value = true;
};

template<typename T>
struct IsHexadecimalValue {
    static constexpr bool value = false;
};

template<typename T>
struct IsHexadecimalValue<HexadecimalValue<T>>
{
    static constexpr bool value = true;
};

#endif // KERNEL_LIB_NUMBERS_H_