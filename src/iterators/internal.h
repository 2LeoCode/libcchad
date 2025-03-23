#ifndef CCHAD_ITERATORS_INTERNAL_H
#define CCHAD_ITERATORS_INTERNAL_H

#include <cchad/allocators.h>
#include <cchad/iterators.h>

struct iterator {
  void* iterable;
  const Allocator allocator;
  IteratorAdvanceFunc* const advance;
  IteratorCleanupFunc* const cleanup;
};

#endif  // !CCHAD_ITERATORS_INTERNAL_H
