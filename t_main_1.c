#include "square_matrix.h"
#include "mm_in.h"

#include "mm_in.c"
#include "square_matrix.c"

// Ensure that mm_in, mm_out, and mm_hybrid produce the same result
int main(void) {
  int prob_width = BASE_WIDTH * 2;
  struct SquareMatrix x = allocate_matrix(prob_width);
  struct SquareMatrix y = allocate_matrix(prob_width);

  int prob_size = prob_width * prob_width;
  for(int i = 0; i < prob_width; i++) {
    for(int j = 0; j < prob_width; j++) {
      int index = i * prob_width + j;
      x.data[index] = index;
      y.data[index] = index + prob_size;
    }
  }

  struct SquareMatrix z1 = allocate_matrix(prob_width);
  struct SquareMatrix z2 = allocate_matrix(prob_width);

  mm_out(&x, &y, &z2);
  mm_in(&x, &y, &z1);

  for(int i = 0; i < prob_size; i++) {
    if (i % prob_width == 0) {
      printf("\n");
    }
    int diff = z1.data[i] - z2.data[i];
    printf("%d, ", diff);
  }
}

