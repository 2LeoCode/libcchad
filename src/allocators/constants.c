#include <stdlib.h>

#include <cchad/allocators.h>

const Allocator systemAllocator = {
    .malloc = malloc,
    .free = free,
    .realloc = realloc,
};
