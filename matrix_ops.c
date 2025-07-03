#ifndef MATRIX_OPS_C
#define MATRIX_OPS_C

#include "matrix_ops.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"
#include "cilk.h"

// Add b to a
void mm_add(struct SquareMatrix* x, struct SquareMatrix* y) {
  assert(x->width == y->width);
  assert(x->data != NULL);
  assert(y->data != NULL);

  for (int i = 0; i < x->width * x->width; i++) {
    x->data[i] += y->data[i];
  }
}

// Assume row major order
void mm_seq(struct SquareMatrix* x,
            struct SquareMatrix* y,
            struct SquareMatrix* z) {
  assert(x->width == y->width);
  assert(y->width == z->width);
  assert(x->data != NULL);
  assert(y->data != NULL);
  assert(z->data != NULL);

  int width = x->width;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      int z_index = i * width + j;
      for (int k = 0; k < width; k++) {
        int x_index = i * width + k;
        int y_index = k * width + j;
        z->data[z_index] += x->data[x_index] * y->data[y_index];
      }
    }
  }
}

void mm_in(struct SquareMatrix* x,
           struct SquareMatrix* y,
           struct SquareMatrix* z) {
  assert(x->width == y->width);
  assert(x->width == z->width);
  assert(x->data != NULL);
  assert(y->data != NULL);
  assert(z->data != NULL);

  if (x->width <= BASE_WIDTH) {
    mm_seq(x, y, z);
    return;
  }

  struct SquareMatrix x_11 = quadrant_11(x);
  struct SquareMatrix x_12 = quadrant_12(x);
  struct SquareMatrix x_21 = quadrant_21(x);
  struct SquareMatrix x_22 = quadrant_22(x);

  struct SquareMatrix y_11 = quadrant_11(y);
  struct SquareMatrix y_12 = quadrant_12(y);
  struct SquareMatrix y_21 = quadrant_21(y);
  struct SquareMatrix y_22 = quadrant_22(y);

  struct SquareMatrix z_11 = quadrant_11(z);
  struct SquareMatrix z_12 = quadrant_12(z);
  struct SquareMatrix z_21 = quadrant_21(z);
  struct SquareMatrix z_22 = quadrant_22(z);

  cilk_spawn mm_in(&x_11, &y_11, &z_11);
  cilk_spawn mm_in(&x_11, &y_12, &z_12);
  cilk_spawn mm_in(&x_21, &y_11, &z_21);
  cilk_spawn mm_in(&x_21, &y_12, &z_22);
  cilk_sync;

  cilk_spawn mm_in(&x_12, &y_21, &z_11);
  cilk_spawn mm_in(&x_12, &y_22, &z_12);
  cilk_spawn mm_in(&x_22, &y_21, &z_21);
  cilk_spawn mm_in(&x_22, &y_22, &z_22);
  cilk_sync;
}

void mm_out(struct SquareMatrix* x,
            struct SquareMatrix* y,
            struct SquareMatrix* z) {
  // printf("mm_out: %d\n", x->width);
  assert(x->width == y->width);
  assert(x->width == z->width);
  assert(x->data != NULL);
  assert(y->data != NULL);
  assert(z->data != NULL);

  if (x->width <= BASE_WIDTH) {
    mm_seq(x, y, z);
    return;
  }

  int prob_size = x->width * x->width;
  struct SquareMatrix t = allocate_matrix(x->width);
  if (t.data == NULL) {
    printf("ERROR: allocation failed\n");
    exit(1);
  }

  struct SquareMatrix x_11 = quadrant_11(x);
  struct SquareMatrix x_12 = quadrant_12(x);
  struct SquareMatrix x_21 = quadrant_21(x);
  struct SquareMatrix x_22 = quadrant_22(x);

  struct SquareMatrix y_11 = quadrant_11(y);
  struct SquareMatrix y_12 = quadrant_12(y);
  struct SquareMatrix y_21 = quadrant_21(y);
  struct SquareMatrix y_22 = quadrant_22(y);

  struct SquareMatrix z_11 = quadrant_11(z);
  struct SquareMatrix z_12 = quadrant_12(z);
  struct SquareMatrix z_21 = quadrant_21(z);
  struct SquareMatrix z_22 = quadrant_22(z);

  struct SquareMatrix t_11 = quadrant_11(&t);
  struct SquareMatrix t_12 = quadrant_12(&t);
  struct SquareMatrix t_21 = quadrant_21(&t);
  struct SquareMatrix t_22 = quadrant_22(&t);

  // Parallel
  cilk_spawn mm_out(&x_11, &y_11, &z_11);
  cilk_spawn mm_out(&x_11, &y_12, &z_12);
  cilk_spawn mm_out(&x_21, &y_11, &z_21);
  cilk_spawn mm_out(&x_21, &y_12, &z_22);
  cilk_spawn mm_out(&x_12, &y_21, &t_11);
  cilk_spawn mm_out(&x_12, &y_22, &t_12);
  cilk_spawn mm_out(&x_22, &y_21, &t_21);
  cilk_spawn mm_out(&x_22, &y_22, &t_22);
  cilk_sync;

  mm_add(z, &t);
  free_matrix(&t);
}

void mm_hybrid(struct SquareMatrix* x,
               struct SquareMatrix* y,
               struct SquareMatrix* z) {
  assert(x->width == y->width);
  assert(x->width == z->width);
  assert(x->data != NULL);
  assert(y->data != NULL);
  assert(z->data != NULL);

  if (x->width <= BASE_WIDTH) {
    mm_seq(x, y, z);
    return;
  }

  struct SquareMatrix x_11 = quadrant_11(x);
  struct SquareMatrix x_12 = quadrant_12(x);
  struct SquareMatrix x_21 = quadrant_21(x);
  struct SquareMatrix x_22 = quadrant_22(x);

  struct SquareMatrix y_11 = quadrant_11(y);
  struct SquareMatrix y_12 = quadrant_12(y);
  struct SquareMatrix y_21 = quadrant_21(y);
  struct SquareMatrix y_22 = quadrant_22(y);

  struct SquareMatrix z_11 = quadrant_11(z);
  struct SquareMatrix z_12 = quadrant_12(z);
  struct SquareMatrix z_21 = quadrant_21(z);
  struct SquareMatrix z_22 = quadrant_22(z);

  struct SquareMatrix t = allocate_matrix(x->width);
  if (t.data == NULL) {
    cilk_spawn mm_hybrid(&x_11, &y_11, &z_11);
    cilk_spawn mm_hybrid(&x_11, &y_12, &z_12);
    cilk_spawn mm_hybrid(&x_21, &y_11, &z_21);
    cilk_spawn mm_hybrid(&x_21, &y_12, &z_22);
    cilk_sync;

    cilk_spawn mm_hybrid(&x_12, &y_21, &z_11);
    cilk_spawn mm_hybrid(&x_12, &y_22, &z_12);
    cilk_spawn mm_hybrid(&x_22, &y_21, &z_21);
    cilk_spawn mm_hybrid(&x_22, &y_22, &z_22);
    cilk_sync;
  } else {
    struct SquareMatrix t_11 = quadrant_11(&t);
    struct SquareMatrix t_12 = quadrant_12(&t);
    struct SquareMatrix t_21 = quadrant_21(&t);
    struct SquareMatrix t_22 = quadrant_22(&t);

    // Parallel
    cilk_spawn mm_hybrid(&x_11, &y_11, &z_11);
    cilk_spawn mm_hybrid(&x_11, &y_12, &z_12);
    cilk_spawn mm_hybrid(&x_21, &y_11, &z_21);
    cilk_spawn mm_hybrid(&x_21, &y_12, &z_22);
    cilk_spawn mm_hybrid(&x_12, &y_21, &t_11);
    cilk_spawn mm_hybrid(&x_12, &y_22, &t_12);
    cilk_spawn mm_hybrid(&x_22, &y_21, &t_21);
    cilk_spawn mm_hybrid(&x_22, &y_22, &t_22);
    cilk_sync;

    mm_add(z, &t);
    free_matrix(&t);
  }
}

int mm_out_memory_usage(int base, int power) {
  printf("mm_out_memory_usage, base: %d, power: %d\n", base, power);
  int width = base * pow(2, power);
  int size = width * width;
  return power * size;
}

#endif
