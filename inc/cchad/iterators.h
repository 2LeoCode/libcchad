#ifndef CCHAD_ITERATORS_H
#define CCHAD_ITERATORS_H

#include <stddef.h>

#include "allocators.h"

typedef struct iterator Iterator;

typedef void* IteratorAdvanceFunc(void* iterable);
typedef void IteratorCleanupFunc(void* iterable);

typedef struct iteratorNewOptions IteratorNewOptions;

/**
 * Create a new iterator.
 *
 * Take an iterable pointer that acts like a cursor to
 * a target object that the user wants to iterate on and
 * an advance function which job will be to return the
 * next element of the target, and advance the iterable
 * by one upon invocation.
 *
 * See IteratorNewOptions for optional parameters.
 *
 * If the end of the iterable is reached, the advance
 * function must return nullptr.
 *
 * It's up to the user to implement its own iteration logic
 * depending on the type of the iterable pointer.
 */
Iterator* Iterator_new(void* iterable,
                       IteratorAdvanceFunc* advance,
                       const IteratorNewOptions* options);

struct iteratorNewOptions {
  /**
   * A cleanup function that will automatically be called
   * upon iterator's consumation.
   */
  IteratorCleanupFunc* cleanup;

  /**
   * An allocator that will be used to manage the iterator's
   * internal memory.
   */
  const Allocator* allocator;
};

/**
 * Get mutable iterator's underlying allocator.
 */
const Allocator* Iterator_getAllocator(const Iterator* iter);

/**
 * Advance the iterator by one and get the next underlying
 * element.
 *
 * Effectively call the underlying advance function, then
 * if it returns nullptr, consume the iterator.
 *
 * The element is returned as immutable.
 */
const void* Iterator_next(Iterator* iter);

/**
 * Advance the iterator by one and get the next underlying
 * element.
 *
 * Effectively call the underlying advance function, then
 * if it returns nullptr, consume the iterator.
 *
 * The element is returned as mutable, use this function only
 * if you are sure that the internal iterable elements are
 * mutable.
 */
void* Iterator_nextMut(Iterator* iter);

/**
 * Create a new mutable iterator over the first n elements of iter.
 *
 * Consume iter in the process.
 */
Iterator* Iterator_take(Iterator* iter, size_t n);

/**
 * Create a new mutable iterator over every element of iter except
 * the first n elements.
 *
 * Any future access to iter becomes invalid, and when the newly created
 * iterator is consumed, the previous iterator is also automatically
 * consumed (think of it as a conversion).
 */
Iterator* Iterator_skip(Iterator* iter, size_t n);

/**
 * Force a mutable iterator to consume itself and free its memory.
 *
 * In most cases you don't need to call this function because Iterator_next
 * automatically consumes the iterator when reaching its end, but it can
 * be useful if for example you need to quit early.
 *
 * It is however invalid and undefined behavior to call this function
 * after the iterator's end as been reached for the above reason.
 */
void Iterator_destroy(Iterator* iter);

#endif  // !CCHAD_ITERATORS_H
