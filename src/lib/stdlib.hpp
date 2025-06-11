#pragma once

#include <limits>
#include <algorithm>

#include "types.hpp"
#include "globalDefs.hpp"

ASMDefined void* gNp_MallocHi(u32 alignment, u32 size);
ASMDefined void free_Hi(void*);

//ASMDefined void* aligned_calloc(u32 alignment, u32 size);
ASMDefined void* aligned_malloc(u32 alignment, u32 size);
ASMDefined void nnFree(void*);
ASMDefined void free_all(void*);
//ASMDefined char* strcpy(char* destination, const char* source);
//ASMDefined void* memcpy(void *dest, const void * src, u32 n);
//ASMDefined void *memset(void *str, u32 value, u32 size);

/**
 * Returns a random value between 0 and max (exclusive)
 */
ASMDefined u32 lbl_RNG_Number(u32 max);

ASMDefined [[noreturn]] void ErrorTrap();

template<typename T>
[[nodiscard]]
constexpr T clamp(const T &num, const T &min = static_cast<T>(0), const T &max = std::numeric_limits<T>::max()){
	return std::clamp(num, min, max); // Just a wrapper around standard clamp that gives default values
}

template<typename T>
[[nodiscard]]
constexpr T clamp(const volatile T &num, const T &min = static_cast<T>(0), const T &max = std::numeric_limits<T>::max()){
	const auto val = num;
	return std::clamp(val, min, max);
}
