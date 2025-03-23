#include "internal.h"

void Iterator_destroy(Iterator* const iter) {
  iter->cleanup(iter->iterable);
  iter->allocator.free(iter);
}
