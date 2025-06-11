#include "stdlib.hpp"
#include "types.hpp"
#include <ogc/machine/processor.h>

// new operator overload
void *operator new(size_t size){
	void *ptr = aligned_malloc(4, size);
	//if(ptr == nullptr){ throw std::bad_alloc(); }
	return ptr;
}

// new operator overload
void *operator new(size_t count, std::align_val_t al = std::align_val_t{4}){
	void *ptr = aligned_malloc(static_cast<size_t>(al), count);
	//if(ptr == nullptr){ throw std::bad_alloc(); }
	return ptr;
}

void operator delete(void *ptr) noexcept{
	nnFree(ptr);
}

void operator delete(void *ptr, unsigned int) noexcept{
	nnFree(ptr);
}

// new[] operator overload
void *operator new[](size_t size){
	void *ptr = aligned_malloc(4, size);
	//if(ptr == nullptr){ throw std::bad_alloc(); }
	return ptr;
}

// new[] operator overload
void *operator new[](size_t size, std::align_val_t al = std::align_val_t{4}){
	void *ptr = aligned_malloc(static_cast<size_t>(al), size);
	//if(ptr == nullptr){ throw std::bad_alloc(); }
	return ptr;
}

void operator delete[](void *ptr) noexcept{
	nnFree(ptr);
}

void operator delete[](void *ptr, unsigned int) noexcept{
	nnFree(ptr);
}

/**
 * A wrapper around the Standard C function atexit
 * Normally this is used by C++ to call the global destructors when exiting the program
 * However "exiting" the program is just cutting the power so this is unnecessary
 * And just causes errors.
 * This effectively just dummies out the original function
 */
ASMUsed int __wrap_atexit(void __attribute__((unused)) (*function)(void)){ //NOLINT
	return -1;
}

/**
 * Wrapper function for the abort call
 * Normally this signals the program to exit (due to some sort of abnormality) but for us this just causes other issues
 * Now we can do custom logic in case of an error. Here we just spin the CPU so we can easily see in the callstack where the issue happened
 */
ASMUsed [[noreturn]] void __wrap_abort(){ //NOLINT
	ppchalt();
}