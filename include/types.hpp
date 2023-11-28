/** @file types.hpp
 * Header for type definitions used throughout the code
 */

#pragma once

// Not sure what defines this or why but this sets it back to the C++ default
#undef bool

#include <concepts>
#include <array>

// Use regular types for regular uses and constants
using s8 = signed char;
using s16 = short;
using s32 = int;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;

using f32 = float;
using f64 = double;

// Volatile types should only be used for external (I.E. ASM or System) variables where the value could change without C++ knowing it
// For example, the variable that contains the game frame count is volatile. Copies of volatile variables should use the regular type
using vs8 = volatile signed char;
using vs16 = volatile short;
using vs32 = volatile int;

using vu8 = volatile unsigned char;
using vu16 = volatile unsigned short;
using vu32 = volatile unsigned int;

using vf32 = volatile float;
using vf64 = volatile double;

// Highly doubt these will be used but defining them anyway
using u64 = unsigned long long;
using s64 = signed long long;

using vu64 = volatile unsigned long long;
using vs64 = volatile signed long long;

using Task = void (*)();
#define TRUE true
#define FALSE false

template<typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

/**
 * Simplified way to specify a 2D std::array.
 * Normally the dimensions are backwards because of the way you have to define it
 * This changes it back to be the same order as a C-Style 2D array
 * Used as <code>m2darray<Type, X, Y></code>, equivalent to <code>Type[X][Y]</code>
 */
template<typename T, std::size_t X, std::size_t Y>
using m2darray = std::array<std::array<T, Y>, X>;

// Adds a simple way to define an array of bytes that are only used as padding for a struct
template<std::size_t size, typename T = std::byte>
using fillerData = std::array<T, size>;