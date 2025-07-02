#include "square_matrix.h"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "matrix_ops.c"

// Ensure that mm_in, mm_out, and mm_hybrid produce the same result
int main(void) {
  int prob_width = BASE_WIDTH * 2;
  struct SquareMatrix x = allocate_matrix(prob_width);

  int prob_size = prob_width * prob_width;
  for(int i = 0; i < prob_width; i++) {
    for(int j = 0; j < prob_width; j++) {
      int index = i * prob_width + j;
      x.data[index] = index;
    }
  }
/*
  struct SquareMatrix x_11 = quadrant_11(&x);
  struct SquareMatrix x_12 = quadrant_12(&x);
  struct SquareMatrix x_21 = quadrant_21(&x);
  struct SquareMatrix x_22 = quadrant_22(&x);
  printf("x_11\n");
  print_row_major_matrix(&x_11);
  printf("x_12\n");
  print_row_major_matrix(&x_12);
  printf("x_21\n");
  print_row_major_matrix(&x_21);
  printf("x_22\n");
  print_row_major_matrix(&x_22);
*/

  printf("First\n");
  print_row_major_matrix(&x);

  struct SquareMatrix y = allocate_matrix(prob_width);
  to_hybrid_major(&x, &y);
  printf("Hybrid\n");
  print_row_major_matrix(&y);
  struct SquareMatrix z = allocate_matrix(prob_width);
  to_row_major(&y, &z);
  printf("Row\n");
  print_row_major_matrix(&z);
}

