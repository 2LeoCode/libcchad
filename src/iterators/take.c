#include <assert.h>
#include <string.h>

#include <cchad/iterators.h>

typedef struct iterable {
  Iterator* base;
  size_t n;
} Iterable;

static void* advance(void* const rawIterable) {
  Iterable* const iterable = rawIterable;

  if (iterable->n == 0) {
    return nullptr;
  }
  --iterable->n;

  void* const result = Iterator_nextMut(iterable->base);
  if (result == nullptr) {
    iterable->base = nullptr;
  }
  return result;
}

static void cleanup(void* const rawIterable) {
  Iterable* const iterable = rawIterable;
  const Allocator* const allocator = Iterator_getAllocator(iterable->base);

  Iterator_destroy(iterable->base);
  allocator->free(iterable);
}

Iterator* Iterator_take(Iterator* const iter, const size_t n) {
  assert(iter != nullptr && "iter cannot be null");

  const Allocator* const allocator = Iterator_getAllocator(iter);
  Iterable* const iterable = allocator->malloc(sizeof *iterable);
  if (iterable == nullptr) {
    return nullptr;
  }

  memcpy(iterable,
         &(typeof(*iterable)){
             .base = iter,
             .n = n,
         },
         sizeof *iterable);

  const IteratorNewOptions options = {
      .allocator = allocator,
      .cleanup = cleanup,
  };

  return Iterator_new(iterable, advance, &options);
}
