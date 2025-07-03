#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "allocator.h"
#include "matrix_ops.c"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "square_matrix.h"

long get_time() {
  struct timeval tp;

  gettimeofday(&tp, NULL);

  long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

  return ms;
}

void print_help() {
  printf(
      "mem_sched_demo O B P M N T\n"
      "Matrix width is B * 2^P\n"
      "O output file\n"
      "B Base matrix width\n"
      "P Power of 2 for problem size\n"
      "M Memory usage limit in ints\n"
      "N Trials to run\n"
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
  // O B P M N T"
  if (argc != 7) {
    printf("ERROR: unexpected argc: %d\n", argc);
    print_help();
    return 1;
  }
  char* output_path = argv[1];
  int base = atoi(argv[2]);
  int power = atoi(argv[3]);
  size_t storage_limit = 0;
  if (strcmp(argv[4], "max") == 0) {
    printf("use max\n");
    storage_limit = (size_t)SIZE_MAX;
  } else if (argv[4][0] == '0') {
    printf("use float, %s\n", argv[4]);
    size_t max_usage = mm_out_memory_usage(base, power);
    char* end = NULL;
    double factor = strtod(argv[4], &end);
    storage_limit = (size_t) (factor * (double) max_usage);
  } else {
    printf("parse limit (%s)\n", argv[4]);
    storage_limit = atoi(argv[4]);
  }
  int n_trials = atoi(argv[5]);
  int type = atoi(argv[6]);
  
  BASE_WIDTH = base;
  int matrix_width = BASE_WIDTH * pow(2, power);
  size_t matrix_size = (size_t) matrix_width * (size_t) matrix_width;
  size_t problem_base_storage = 3 * matrix_size;

  printf("output_path: %s\n", output_path);
  printf("base: %d\n", base);
  printf("power: %d\n", power);
  printf("width: %d\n", matrix_width);
  printf("size: %zu\n", matrix_size);
  printf("storage_limit: %zu\n", storage_limit);
  printf("type: %s\n", mm_type(type));
  zero_storage();
  set_int_storage_limit(storage_limit);
  struct SquareMatrix x = allocate_matrix(matrix_width);
  struct SquareMatrix y = allocate_matrix(matrix_width);
  struct SquareMatrix z = allocate_matrix(matrix_width);
  assert(total_ints_stored() == problem_base_storage);

  // Set ICs
  for (int i = 0; i < matrix_width; i++) {
    for (int j = 0; j < matrix_width; j++) {
      int index = i * matrix_width + j;
      x.data[index] = index;
      y.data[index] = index + matrix_size;
    }
  }

  FILE* fp = fopen(output_path, "w");
  if (fp == NULL) {
    printf("ERROR: Failed to open file");
    exit(1);
  }
  for (int trial = 0; trial < n_trials; trial++) {
    printf("Running trial: %d...\n", trial);
    set_int_storage(problem_base_storage);
    long start = get_time();
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
    long end = get_time();
    double elapsed_s = (end - start) / 1000.0;
    size_t usage = total_ints_stored();
    fprintf(fp, "%zu %f %zu\n", matrix_size, elapsed_s, usage);
    printf("Usage: %zu\n", usage);
  }
  fclose(fp);

  free_matrix(&x);
  free_matrix(&y);
  free_matrix(&z);
  printf("Done, z[0]: %d\n", z.data[0]);
}
