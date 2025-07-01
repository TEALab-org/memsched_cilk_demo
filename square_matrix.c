#ifndef SQUARE_MATRIX_C
#define SQUARE_MATRIX_C

#include "square_matrix.h"

struct SquareMatrix allocate_matrix(int width) {
  int size = width * width;
  double* data = malloc(size * sizeof(double));
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

#endif
