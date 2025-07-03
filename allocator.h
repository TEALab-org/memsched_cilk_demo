#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

atomic_int total_mem = 0;

int MEM_USAGE_LIMIT = 0;

int* int_malloc(int n_ints) {
  //int bytes = n_ints * sizeof(int);
  int afae = atomic_fetch_add_explicit(&total_mem, n_ints, memory_order_relaxed);
  int a_total_mem = atomic_load(&total_mem);
  printf("int_malloc -- n_ints: %d, afae: %d, total_mem: %d\n", n_ints, afae, total_mem);
  if ( afae + n_ints > MEM_USAGE_LIMIT) {
    atomic_fetch_add_explicit(&total_mem, -n_ints, memory_order_relaxed);
    printf("  Alloc Failed, total_mem: %d, limit: %d\n", a_total_mem, MEM_USAGE_LIMIT);
    return NULL; 
  }
  printf("  Alloc Success \n");
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
