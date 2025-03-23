#include <assert.h>

#include "internal.h"

const void* Iterator_next(Iterator* const iter) {
  assert(iter != nullptr && "iter cannot be null");

  void* const result = iter->advance(iter->iterable);
  if (result == nullptr) {
    Iterator_destroy(iter);
  }

  return result;
}

void* Iterator_nextMut(Iterator* const iter) {
  assert(iter != nullptr && "iter cannot be null");

  void* const result = iter->advance(iter->iterable);
  if (result == nullptr) {
    Iterator_destroy(iter);
  }

  return result;
}
