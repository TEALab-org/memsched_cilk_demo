#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

atomic_int total_mem = 0;

int MEM_USAGE_LIMIT = 0;

int* int_malloc(int n_ints) {
  // int bytes = n_ints * sizeof(int);
  int pre_total_ints_stored =
      atomic_fetch_add_explicit(&total_mem, n_ints, memory_order_relaxed);
  if (pre_total_ints_stored + n_ints > MEM_USAGE_LIMIT) {
    atomic_fetch_add_explicit(&total_mem, -n_ints, memory_order_relaxed);
    return NULL;
  }
  int* result = calloc(n_ints, sizeof(int));
  return result;
}

void zero_storage() {
  atomic_store(&total_mem, 0.0);
}

void print_total_ints_stored() {
  int total_used = atomic_load(&total_mem);
  printf("Total bytes allocated: %d\n", total_used);
}

int total_ints_stored() {
  int total_used = atomic_load(&total_mem);
  return total_used;
}

void set_int_storage_limit(int limit) {
  MEM_USAGE_LIMIT = limit;
}

#endif
