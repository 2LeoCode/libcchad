#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/new/assert.h>

#include <stdlib.h>

#include <cchad/iterators.h>
#include <string.h>

size_t allocCnt = 0;

void* testMalloc(size_t size) {
  void* ptr = malloc(size);
  ++allocCnt;
  return ptr;
}

void testFree(void* ptr) {
  free(ptr);
  --allocCnt;
}

Allocator testAllocator = {
    .malloc = testMalloc,
    .free = testFree,
    .realloc = realloc,
};

void* testAdvance(void* iterable) {
  char** iterStr = iterable;

  if (**iterStr) {
    return (*iterStr)++;
  }
  return nullptr;
}

void testCleanup(void* iterable) {
  char** ptr = iterable;
  while (**ptr != '$') {
    --*ptr;
  }
  testFree(*ptr);
}

Test(iterator, next) {
  char* s = testMalloc(8);
  memcpy(s, "$Hello!", 8);

  char* p = s;
  const char* c;

  IteratorNewOptions testOptions = {
      .cleanup = testCleanup,
      .allocator = &testAllocator,
  };

  auto iter = Iterator_new(&p, testAdvance, &testOptions);

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, '$')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'H')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'e')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'l')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'l')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'o')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, '!')));

  c = Iterator_next(iter);
  cr_expect(eq(ptr, (void*)c, nullptr));

  cr_expect(eq(sz, allocCnt, 0));
}

Test(iterator, take) {
  char* s = testMalloc(8);
  memcpy(s, "$Hello!", 8);

  char* p = s;
  const char* c;

  IteratorNewOptions testOptions = {
      .cleanup = testCleanup,
      .allocator = &testAllocator,
  };

  auto iter = Iterator_new(&p, testAdvance, &testOptions);
  iter = Iterator_take(iter, 3);

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, '$')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'H')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'e')));

  c = Iterator_next(iter);
  cr_expect(eq(ptr, (void*)c, nullptr));

  cr_expect(eq(sz, allocCnt, 0));
}

Test(iterator, skip) {
  char* s = testMalloc(8);
  memcpy(s, "$Hello!", 8);

  char* p = s;
  const char* c;

  IteratorNewOptions testOptions = {
      .cleanup = testCleanup,
      .allocator = &testAllocator,
  };

  auto iter = Iterator_new(&p, testAdvance, &testOptions);
  iter = Iterator_skip(iter, 4);

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'l')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, 'o')));

  c = Iterator_next(iter);
  cr_expect(all(not(eq(ptr, (void*)c, nullptr)), eq(chr, *c, '!')));

  c = Iterator_next(iter);
  cr_expect(eq(ptr, (void*)c, nullptr));

  cr_expect(eq(sz, allocCnt, 0));
}
