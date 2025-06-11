/** @file globalDefs.hpp
 * Header for general globally used things.
 */

#pragma once

#include "shared-definitions.h"
#include "types.hpp"
constexpr f32 SPEED_DIVISOR = 216.0F; // value used as a divisor to calculate player speed in game

constexpr auto MaxPlayerCount = 8;
constexpr auto MaxControllerCount = 4;

/**
 * Converts the speed that would be shown on the speedometer into the internal speed values used
 * @param n Speedometer speed
 * @return Internal speed
 */
constexpr f32 pSpeed(const f32 &n) {
	return n / SPEED_DIVISOR;
}

template<typename T> requires arithmetic<T>
constexpr f32 toFrames(const T &n){
	return static_cast<f32>(n) * 60.0F; // NOLINT(readability-magic-numbers)
}

/// Disables C++'s name mangling so ASM can see this as a normal symbol
/// Needed when something is: Defined in ASM, used by C++
#define ASMDefined extern "C"

/// Tells GCC that this thing must not be optimized away, such as when its only used in Assembly
#define USED __attribute((used))

/// Marks this function as one that asm calls
/// Needed when something is: Used by ASM, Defined in C++
/// Note: NOT needed when used by *inline* asm as extended asm input
#define ASMUsed ASMDefined USED