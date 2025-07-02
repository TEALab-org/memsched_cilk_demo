#include <math.h>

#include "allocator.h"
#include "matrix_ops.c"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "square_matrix.h"

void print_help() {
  printf(
      "mem_sched_demo P M T\n"
      "P Matrix width is BASE_WIDTH * 2^P\n"
      "M Memory usage limit in bytes\n"
      "T is mm type, 0 => In place\n"
      "              1 => Out of place\n"
      "              2 => Hybrid\n");
}

char* mm_type(int type) {
  switch (type) {
    case 0:
      return "in-place";
    case 1:
      return "out-of-place";
    case 2:
      return "hybrid";
    default:
      return "ERROR";
  }
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    print_help();
    return 1;
  }

  int power = atoi(argv[0]);
  MEM_USAGE_LIMIT = atoi(argv[1]);
  int type = atoi(argv[1]);

  int matrix_width = BASE_WIDTH * pow(2, power);

  printf("BASE_WIDTH: %d, Matrix Width: %d, Mem Usage Limit: %d, type: %s\n",
         BASE_WIDTH, matrix_width, MEM_USAGE_LIMIT, mm_type(type));

  struct SquareMatrix x = allocate_matrix(matrix_width);
  struct SquareMatrix y = allocate_matrix(matrix_width);
  struct SquareMatrix z = allocate_matrix(matrix_width);

  let matrix_size = matrix_width * matrix_width;
  for (int i = 0; i < matrix_width; i++) {
    for (int j = 0; j < matrix_width; j++) {
      int index = i * matrix_width + j;
      x.data[index] = index;
      y.data[index] = index + matrix_size;
    }
  }

  switch (type) {
    case 0:
      mm_in(&x, &y, &z);
      break;
    case 1:
      mm_out(&x, &y, &z);
      break;
    case 2:
      mm_hybrid(&x, &y, &z);
      break;
    default: 
      printf("ERROR\n");
  }

  printf("Done, z[0]: %d\n", z.data[0]);
}
