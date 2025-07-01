#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

int BASE_WIDTH = 4;

struct SquareMatrix {
  double* data;
  int width;
};

// Allocate memory for a new matrix
struct SquareMatrix allocate_matrix(int width);

// Free memory allocated for a matrix
// WARNING: only use on matrices created by allocate_matrix
void free_matrix(struct SquareMatrix* m);

// Return first quadrant of m
struct SquareMatrix quadrant_11(struct SquareMatrix* m);

// Return second quadrant of m
struct SquareMatrix quadrant_12(struct SquareMatrix* m);

// Return third quadrant of m
struct SquareMatrix quadrant_21(struct SquareMatrix* m);

// Return fourth quadrant of m
struct SquareMatrix quadrant_22(struct SquareMatrix* m);

#endif
