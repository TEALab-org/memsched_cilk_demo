#include "square_matrix.h"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "matrix_ops.c"
#include "allocator.h"

// Ensure that mm_in, mm_out, and mm_hybrid produce the same result
int main(void) {
  int prob_width = BASE_WIDTH * 4;
  struct SquareMatrix x_mm_seq = allocate_matrix(prob_width);
  struct SquareMatrix x_mm_in = allocate_matrix(prob_width);
  struct SquareMatrix x_mm_out = allocate_matrix(prob_width);

  struct SquareMatrix y_mm_seq = allocate_matrix(prob_width);
  struct SquareMatrix y_mm_in = allocate_matrix(prob_width);
  struct SquareMatrix y_mm_out = allocate_matrix(prob_width);

  struct SquareMatrix z_mm_seq = allocate_matrix(prob_width);
  struct SquareMatrix z_mm_in = allocate_matrix(prob_width);
  struct SquareMatrix z_mm_out = allocate_matrix(prob_width);

  int prob_size = prob_width * prob_width;
  for(int i = 0; i < prob_width; i++) {
    for(int j = 0; j < prob_width; j++) {
      int index = i * prob_width + j;
      x_mm_seq.data[index] = index;
      y_mm_seq.data[index] = index + prob_size;
      x_mm_in.data[index] = index;
      y_mm_in.data[index] = index + prob_size;
      x_mm_out.data[index] = index;
      y_mm_out.data[index] = index + prob_size;
    }
  }

  // seq
  mm_seq(&x_mm_seq, &y_mm_seq, &z_mm_seq);

  // mm_in
  struct SquareMatrix x_hybrid_mm_in = allocate_matrix(prob_width);
  struct SquareMatrix y_hybrid_mm_in = allocate_matrix(prob_width);
  struct SquareMatrix z_hybrid_mm_in = allocate_matrix(prob_width);
  to_hybrid_major(&x_mm_in, &x_hybrid_mm_in);
  to_hybrid_major(&y_mm_in, &y_hybrid_mm_in);
  mm_in(&x_hybrid_mm_in, &y_hybrid_mm_in, &z_hybrid_mm_in);
  to_row_major(&z_hybrid_mm_in, &z_mm_in);

  // mm_out
  struct SquareMatrix x_hybrid_mm_out = allocate_matrix(prob_width);
  struct SquareMatrix y_hybrid_mm_out = allocate_matrix(prob_width);
  struct SquareMatrix z_hybrid_mm_out = allocate_matrix(prob_width);
  to_hybrid_major(&x_mm_out, &x_hybrid_mm_out);
  to_hybrid_major(&y_mm_out, &y_hybrid_mm_out);
  mm_out(&x_hybrid_mm_out, &y_hybrid_mm_out, &z_hybrid_mm_out);
  to_row_major(&z_hybrid_mm_out, &z_mm_out);

  for(int i = 0; i < prob_size; i++) {
    int diff_in = z_mm_seq.data[i] - z_mm_in.data[i];
    int diff_out = z_mm_seq.data[i] - z_mm_out.data[i];
    assert(diff_in == 0);
    assert(diff_out == 0);
  }

  printf("Checks passed!\n");
  print_total_mem();
  return 0;
}

