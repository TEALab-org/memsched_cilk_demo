#include "square_matrix.h"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "matrix_ops.c"

// Check that quadrants act as expected 
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

  struct SquareMatrix x_hybrid = allocate_matrix(prob_width);
  to_hybrid_major(&x, &x_hybrid);

  struct SquareMatrix x_11 = quadrant_11(&x_hybrid);
  struct SquareMatrix x_12 = quadrant_12(&x_hybrid);
  struct SquareMatrix x_21 = quadrant_21(&x_hybrid);
  struct SquareMatrix x_22 = quadrant_22(&x_hybrid);
  printf("x_11\n");
  print_row_major_matrix(&x_11);
  printf("x_12\n");
  print_row_major_matrix(&x_12);
  printf("x_21\n");
  print_row_major_matrix(&x_21);
  printf("x_22\n");
  print_row_major_matrix(&x_22);
}

