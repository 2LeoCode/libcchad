#include "internal.h"

const Allocator* Iterator_getAllocator(const Iterator* const iter) {
  return &iter->allocator;
}
