#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

int BASE_WIDTH = 4;

struct SquareMatrix {
  int* data;
  int width;
};

// Allocate memory for a new matrix
struct SquareMatrix allocate_matrix(int width);

// Free memory allocated by a matrix
// WARNING: only use on matrices created by allocate_matrix
// Also does not free the SquareMatrix pointer itself,
// hopefully thats on the stack somewhere :)
void free_matrix(struct SquareMatrix* m);

// Return first quadrant of m
struct SquareMatrix quadrant_11(struct SquareMatrix* m);

// Return second quadrant of m
struct SquareMatrix quadrant_12(struct SquareMatrix* m);

// Return third quadrant of m
struct SquareMatrix quadrant_21(struct SquareMatrix* m);

// Return fourth quadrant of m
struct SquareMatrix quadrant_22(struct SquareMatrix* m);

void to_row_order(struct SquareMatrix* x, struct SquareMatrix* r);

void to_hybrid_order(struct SquareMatrix* x, struct SquareMatrix* r);

void print_base_matrix(struct SquareMatrix* m);

void print_row_order_matrix(struct SquareMatrix* m);
#endif
