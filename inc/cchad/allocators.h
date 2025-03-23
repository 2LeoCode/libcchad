#ifndef CCHAD_ALLOCATORS_H
#define CCHAD_ALLOCATORS_H

#include <stddef.h>

typedef struct allocator Allocator;

typedef void* Allocator_MallocFunc(size_t size);

typedef void Allocator_FreeFunc(void* ptr);

typedef void* Allocator_ReallocFunc(void* ptr, size_t size);

struct allocator {
  Allocator_MallocFunc* const malloc;
  Allocator_FreeFunc* const free;
  Allocator_ReallocFunc* const realloc;
};

/**
 * The system's allocator, used by default when no allocator is provided.
 * Its underlying functions are:
 *  - malloc: stdlib's malloc
 *  - free: stdlib's free
 *  - realloc: stdlib's realloc
 */
extern const Allocator systemAllocator;

#endif  // !CCHAD_ALLOCATORS_H
