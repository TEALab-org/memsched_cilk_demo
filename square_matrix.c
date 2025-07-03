#ifndef SQUARE_MATRIX_C
#define SQUARE_MATRIX_C

#include "square_matrix.h"
#include "allocator.h"

struct SquareMatrix allocate_matrix(int width) {
  int size = width * width;
  int* data = int_malloc(size);
  struct SquareMatrix result;
  result.data = data;
  result.width = width;
  return result;
}

void free_matrix(struct SquareMatrix* m) {
  free(m->data);
}

struct SquareMatrix quadrant_11(struct SquareMatrix* m) {
  struct SquareMatrix result;
  result.data = m->data;
  result.width = m->width / 2;
  return result;
}

struct SquareMatrix quadrant_12(struct SquareMatrix* m) {
  struct SquareMatrix result;
  int quadrant_size = (m->width * m->width) / 4;
  result.data = m->data + quadrant_size;
  result.width = m->width / 2;
  return result;
}

struct SquareMatrix quadrant_21(struct SquareMatrix* m) {
  struct SquareMatrix result;
  int quadrant_size = (m->width * m->width) / 4;
  result.data = m->data + 2 * quadrant_size;
  result.width = m->width / 2;
  return result;
}

struct SquareMatrix quadrant_22(struct SquareMatrix* m) {
  struct SquareMatrix result;
  int quadrant_size = (m->width * m->width) / 4;
  result.data = m->data + 3 * quadrant_size;
  result.width = m->width / 2;
  return result;
}

void print_base_matrix(struct SquareMatrix* m) {
  assert(m->width == BASE_WIDTH);
  for (int i = 0; i < BASE_WIDTH; i++) {
    for (int j = 0; j < BASE_WIDTH; j++) {
      int index = i * BASE_WIDTH + j;
      printf("%d, ", m->data[index]);
    }
    printf("\n");
  }
}

// X is in row order
// R should be hybrid order
// Recursivley break apart R into base size matrices
void to_hybrid_order_rec(int i,
                         int j,
                         struct SquareMatrix* x,
                         struct SquareMatrix* r) {
  if (r->width <= BASE_WIDTH) {
    for (int xi = 0; xi < BASE_WIDTH; xi++) {
      for (int xj = 0; xj < BASE_WIDTH; xj++) {
        int r_index = xi * BASE_WIDTH + xj;
        int x_index = (i + xi) * x->width + (j + xj);
        r->data[r_index] = x->data[x_index];
      }
    }
  } else {
    struct SquareMatrix r_11 = quadrant_11(r);
    struct SquareMatrix r_12 = quadrant_12(r);
    struct SquareMatrix r_21 = quadrant_21(r);
    struct SquareMatrix r_22 = quadrant_22(r);
    int quad_width = r_11.width;

    cilk_spawn to_hybrid_order_rec(i, j, x, &r_11);
    cilk_spawn to_hybrid_order_rec(i, j + quad_width, x, &r_12);
    cilk_spawn to_hybrid_order_rec(i + quad_width, j, x, &r_21);
    cilk_spawn to_hybrid_order_rec(i + quad_width, j + quad_width, x, &r_22);
  }
}

void to_hybrid_order(struct SquareMatrix* x, struct SquareMatrix* r) {
  assert(x->width == r->width);
  to_hybrid_order_rec(0, 0, x, r);
  cilk_sync;
}

void to_row_order_rec(int i,
                      int j,
                      struct SquareMatrix* x,
                      struct SquareMatrix* r) {
  if (x->width <= BASE_WIDTH) {
    for (int xi = 0; xi < BASE_WIDTH; xi++) {
      for (int xj = 0; xj < BASE_WIDTH; xj++) {
        int x_index = xi * BASE_WIDTH + xj;
        int r_index = (i + xi) * r->width + (j + xj);
        r->data[r_index] = x->data[x_index];
      }
    }
  } else {
    struct SquareMatrix x_11 = quadrant_11(x);
    struct SquareMatrix x_12 = quadrant_12(x);
    struct SquareMatrix x_21 = quadrant_21(x);
    struct SquareMatrix x_22 = quadrant_22(x);
    int quad_width = x_11.width;

    cilk_spawn to_row_order_rec(i, j, &x_11, r);
    cilk_spawn to_row_order_rec(i, j + quad_width, &x_12, r);
    cilk_spawn to_row_order_rec(i + quad_width, j, &x_21, r);
    cilk_spawn to_row_order_rec(i + quad_width, j + quad_width, &x_22, r);
  }
}

void to_row_order(struct SquareMatrix* x, struct SquareMatrix* r) {
  assert(x->width == r->width);
  to_row_order_rec(0, 0, x, r);
  cilk_sync;
}

void print_row_order_matrix(struct SquareMatrix* m) {
  for (int i = 0; i < m->width; i++) {
    for (int j = 0; j < m->width; j++) {
      int index = i * m->width + j;
      printf("%d, ", m->data[index]);
    }
    printf("\n");
  }
}

#endif
