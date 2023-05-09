#pragma once

#include "main.hpp"

global void* aligned_malloc(u32 alignment, u32 size);
global void* gNp_MallocHi(u32 alignment, u32 size);
global void* aligned_calloc(u32 alignment, u32 size);
global void free_Hi(void*);
//void sprintf(void*, const char [], ...);
global void nnFree(void*);
global void free_all(void*);
global char* strcpy(char* destination, const char* source);
global void* memcpy(void *dest, const void * src, u32 n);

// OS Functions
global void OSCreateThread(void *thread, Task function, u32, void *stack, u32, u32, u32);
global void OSResumeThread(void *thread);
global void* _OSGetCurrentContext();
global void OSCancelThread(void*);

global void *TRK_memset(void *str, u32 value, u32 size);

global u32 lbl_RNG_Number(u32);

#include <limits>
#include <algorithm>

template<typename T>
[[nodiscard]]
constexpr T clamp(const T &num, const T &min = std::numeric_limits<T>::min(), const T &max = std::numeric_limits<T>::max()){
	return std::clamp(num, min, max); // Just a wrapper around standard clamp that gives default values
}