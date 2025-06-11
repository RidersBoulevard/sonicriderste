#pragma once

#include "types.hpp"
#include <concepts>
#include <type_traits>

template <typename T> requires std::integral<T>
constexpr T align_val(const T val, const u32 alignment) {
    return (val + (alignment - 1)) & ~(alignment - 1);
}

template <typename T> requires std::is_pointer_v<T>
constexpr T align_ptr(const T val, const u32 alignment) {
    return reinterpret_cast<T>((reinterpret_cast<u32>(val) + (alignment - 1)) & ~(alignment - 1));
}