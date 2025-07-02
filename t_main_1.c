#include "square_matrix.h"
#include "mm_in.h"

#include "mm_in.c"
#include "square_matrix.c"

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


  struct SquareMatrix z = allocate_matrix(prob_width);
  mm_in(&x, &y, &z);

  printf("x:\n");
  print_base_matrix(&x);
  printf("y:\n");
  print_base_matrix(&y);
  printf("z:\n");
  print_base_matrix(&z);
}

