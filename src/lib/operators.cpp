#include <cstddef>
#include "context.hpp"

void *operator new(size_t n) {
	// new operator overload
	return gNp_MallocHi(4, n);
}

void operator delete(void *p) {
	free_Hi(p);
}

void operator delete(void *p, unsigned int) {
	free_Hi(p);
}

void *operator new[](size_t n) {
	// new[] operator overload
	return aligned_calloc(4, n);
}

void operator delete[](void *p) {
	nnFree(p); // Todo: check if this is the correct free
}

void operator delete[](void *p, unsigned int) {
	nnFree(p); // Todo: check if this is the correct free
}