#include <assert.h>
#include <string.h>

#include "cchad/utils.h"
#include "internal.h"

Iterator* Iterator_new(void* const iterable,
                       IteratorAdvanceFunc* const advance,
                       const IteratorNewOptions* const options) {
  assert(iterable != nullptr && "iterable cannot be null");
  assert(advance != nullptr && "advance cannot be null");

  IteratorCleanupFunc* cleanup = options->cleanup;
  if (cleanup == nullptr) {
    cleanup = (void (*)(void*))nop;
  }

  const Allocator* allocator = options->allocator;
  if (allocator == nullptr) {
    allocator = &systemAllocator;
  }

  Iterator* const iter = allocator->malloc(sizeof *iter);

  if (iter != nullptr) {
    memcpy(iter,
           &(typeof(*iter)){
               .iterable = iterable,
               .advance = advance,
               .cleanup = cleanup,
               .allocator = *allocator,
           },
           sizeof *iter);
  }

  return iter;
}
