#include "stdlib.hpp"
#include "types.hpp"

// new operator overload
void *operator new(size_t size) {
	void* ptr = aligned_malloc(4, size);
	if(ptr == nullptr) { throw std::bad_alloc(); }
	return ptr;
}

// new operator overload
void *operator new(size_t count, std::align_val_t al = std::align_val_t { 4 }) {
	void* ptr = aligned_malloc(static_cast<size_t>(al), count);
	if(ptr == nullptr) { throw std::bad_alloc(); }
	return ptr;
}

void operator delete(void *ptr) noexcept{
	nnFree(ptr);
}

void operator delete(void *ptr, unsigned int) noexcept{
	nnFree(ptr);
}

// new[] operator overload
void *operator new[](size_t size) {
	void* ptr = aligned_malloc(4, size);
	if(ptr == nullptr) { throw std::bad_alloc(); }
	return ptr;
}

// new[] operator overload
void *operator new[](size_t size, std::align_val_t al = std::align_val_t { 4 }) {
	void* ptr = aligned_malloc(static_cast<size_t>(al), size);
	if(ptr == nullptr) { throw std::bad_alloc(); }
	return ptr;
}

void operator delete[](void *ptr) noexcept{
	nnFree(ptr);
}

void operator delete[](void *ptr, unsigned int) noexcept{
	nnFree(ptr);
}