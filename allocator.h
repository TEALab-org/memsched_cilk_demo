#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

atomic_int total_mem = 0;

int MEM_USAGE_LIMIT = 0;

void* t_malloc(int bytes) {
  atomic_fetch_add_explicit(&total_mem, bytes, memory_order_relaxed);
  return malloc(bytes);
}

void zero_total_mem() {
  atomic_store(&total_mem, 0.0);
}

void print_total_mem() {
  int total_used = atomic_load(&total_mem);
  printf("Total bytes allocated: %d\n", total_used);
}

int get_total_mem() {
  int total_used = atomic_load(&total_mem);
  return total_used;
}

void set_mem_usage_limit(int limit) {
  MEM_USAGE_LIMIT = limit;
}

#endif
