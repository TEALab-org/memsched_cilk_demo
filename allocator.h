#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

atomic_size_t A_TOTAL_INTS_STORED = 0;

size_t INT_STORAGE_LIMIT = 0;

int* int_malloc(size_t n_ints) {
  int pre_total_ints_stored = atomic_fetch_add_explicit(
      &A_TOTAL_INTS_STORED, n_ints, memory_order_relaxed);
  // printf("int_malloc, pre_fetch: %d, n_ints: %d, limit: %d\n",
  // pre_total_ints_stored, n_ints, INT_STORAGE_LIMIT);
  if (pre_total_ints_stored + n_ints > INT_STORAGE_LIMIT) {
    atomic_fetch_sub_explicit(&A_TOTAL_INTS_STORED, n_ints,
                              memory_order_relaxed);
    return NULL;
  }
  int* result = calloc(n_ints, sizeof(int));
  return result;
}

void int_free(int* data, size_t n_ints) {
  atomic_fetch_sub_explicit(&A_TOTAL_INTS_STORED, n_ints, memory_order_relaxed);
  free(data);
}

void zero_storage() {
  atomic_store(&A_TOTAL_INTS_STORED, 0.0);
}

void set_int_storage(int ints_stored) {
  atomic_store(&A_TOTAL_INTS_STORED, ints_stored);
}

void print_total_ints_stored() {
  size_t ints_stored = atomic_load(&A_TOTAL_INTS_STORED);
  printf("Total bytes allocated: %zu\n", ints_stored);
}

size_t total_ints_stored() {
  size_t total_used = atomic_load(&A_TOTAL_INTS_STORED);
  return total_used;
}

void set_int_storage_limit(size_t limit) {
  INT_STORAGE_LIMIT = limit;
}

#endif
