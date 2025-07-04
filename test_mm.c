// test_mm.c
//
// Just build and run this without arguments.
// In lieu of a testing framework
// We test that
//   * mm_in,
//   * mm_out,
//   * mm_hybrid
// match the output of mm_seq,
// with one x, y pair :(

#include <assert.h>
#include <math.h>
#include "allocator.h"
#include "matrix_ops.c"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "square_matrix.h"

// Sets the values of x and y as each elements linear index (i.e. y_00 = |x|),
// in row order order
void ic(struct SquareMatrix* x, struct SquareMatrix* y) {
  printf("Running ic...\n");
  assert(x->width == y->width);
  assert(x->data != NULL);
  assert(y->data != NULL);
  int width = x->width;
  int size = width * width;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      int index = i * width + j;
      x->data[index] = index;
      y->data[index] = index + size;
    }
  }
}

// Generate expected result with sequential solver
// Must free result!
struct SquareMatrix generate_expected(int width) {
  printf("Running generate_expected...\n");
  int size = width * width;
  int expected_n_ints_stored = size * 3;
  zero_storage();
  set_int_storage_limit(expected_n_ints_stored * 2);
  struct SquareMatrix x_row_order = allocate_matrix(width);
  struct SquareMatrix y_row_order = allocate_matrix(width);
  struct SquareMatrix z_row_order = allocate_matrix(width);
  assert(x_row_order.data != NULL);
  assert(y_row_order.data != NULL);
  assert(z_row_order.data != NULL);
  assert(total_ints_stored() == expected_n_ints_stored &&
         "ERROR: pre Seq mem usage");
  zero_storage();

  ic(&x_row_order, &y_row_order);
  mm_seq(&x_row_order, &y_row_order, &z_row_order);
  assert(total_ints_stored() == 0 && "ERROR: Seq Mem Usage Not Zero");
  free_matrix(&x_row_order);
  free_matrix(&y_row_order);
  printf("Completed Test: Generate Expected Result\n");
  return z_row_order;
}

void test_mm_in(struct SquareMatrix* expected_row_order) {
  printf("Running test_mm_in...\n");
  int width = expected_row_order->width;
  int size = width * width;
  int expected_n_ints_stored = size * 6;
  zero_storage();
  set_int_storage_limit(expected_n_ints_stored * 2);
  struct SquareMatrix x_row_order = allocate_matrix(width);
  struct SquareMatrix y_row_order = allocate_matrix(width);
  struct SquareMatrix z_row_order = allocate_matrix(width);
  struct SquareMatrix x_hybrid_order = allocate_matrix(width);
  struct SquareMatrix y_hybrid_order = allocate_matrix(width);
  struct SquareMatrix z_hybrid_order = allocate_matrix(width);
  assert(x_row_order.data != NULL);
  assert(y_row_order.data != NULL);
  assert(z_row_order.data != NULL);
  assert(x_hybrid_order.data != NULL);
  assert(y_hybrid_order.data != NULL);
  assert(z_hybrid_order.data != NULL);
  assert(total_ints_stored() == expected_n_ints_stored &&
         "ERROR: pre Seq mem usage");
  zero_storage();

  set_int_storage_limit(0);
  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  mm_in(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  assert(total_ints_stored() == 0 && "ERROR: In-place Mem Usage Not Zero");
  to_row_order(&z_hybrid_order, &z_row_order);

  for (int i = 0; i < size; i++) {
    int diff = z_row_order.data[i] - expected_row_order->data[i];
    assert(diff == 0 && "ERROR: In-place did not match expected");
  }

  free_matrix(&x_row_order);
  free_matrix(&y_row_order);
  free_matrix(&z_row_order);
  free_matrix(&x_hybrid_order);
  free_matrix(&y_hybrid_order);
  free_matrix(&z_hybrid_order);
  printf("Completed Test: In-place\n");
}

void test_mm_out(struct SquareMatrix* expected_row_order, int power) {
  printf("Running test_mm_out...\n");
  int width = expected_row_order->width;
  int size = width * width;
  int expected_n_ints_stored = size * 6;
  zero_storage();
  set_int_storage_limit(expected_n_ints_stored * 2);
  struct SquareMatrix x_row_order = allocate_matrix(width);
  struct SquareMatrix y_row_order = allocate_matrix(width);
  struct SquareMatrix z_row_order = allocate_matrix(width);
  struct SquareMatrix x_hybrid_order = allocate_matrix(width);
  struct SquareMatrix y_hybrid_order = allocate_matrix(width);
  struct SquareMatrix z_hybrid_order = allocate_matrix(width);
  assert(x_row_order.data != NULL);
  assert(y_row_order.data != NULL);
  assert(z_row_order.data != NULL);
  assert(x_hybrid_order.data != NULL);
  assert(y_hybrid_order.data != NULL);
  assert(z_hybrid_order.data != NULL);
  assert(total_ints_stored() == expected_n_ints_stored &&
         "ERROR: pre Seq mem usage");
  zero_storage();

  int expected_out_storage = mm_out_memory_usage(BASE_WIDTH, power);
  printf("Expected out storage: %d\n", expected_out_storage);
  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  zero_storage();
  set_int_storage_limit(999999999);
  mm_out(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  to_row_order(&z_hybrid_order, &z_row_order);

  assert(total_ints_stored() != 0 && "ERROR: Out-of-place Mem Usage is Zero");
  printf("mm_out mem report, base: %d, power: %d, used: %zu, expected: %d\n",
         BASE_WIDTH, power, total_ints_stored(), expected_out_storage);

  for (int i = 0; i < size; i++) {
    int diff = z_row_order.data[i] - expected_row_order->data[i];
    assert(diff == 0 && "ERROR: Out-of-place did not match expected");
  }

  free_matrix(&x_row_order);
  free_matrix(&y_row_order);
  free_matrix(&z_row_order);
  free_matrix(&x_hybrid_order);
  free_matrix(&y_hybrid_order);
  free_matrix(&z_hybrid_order);
  printf("Completed Test: Out-of-place\n");
}

void test_mm_hybrid(struct SquareMatrix* expected_row_order,
                    int mem_usage_limit) {
  printf("Running test_mm_hybrid...\n");
  int width = expected_row_order->width;
  int size = width * width;
  int expected_n_ints_stored = size * 6;
  zero_storage();
  set_int_storage_limit(expected_n_ints_stored * 2);
  struct SquareMatrix x_row_order = allocate_matrix(width);
  struct SquareMatrix y_row_order = allocate_matrix(width);
  struct SquareMatrix z_row_order = allocate_matrix(width);
  struct SquareMatrix x_hybrid_order = allocate_matrix(width);
  struct SquareMatrix y_hybrid_order = allocate_matrix(width);
  struct SquareMatrix z_hybrid_order = allocate_matrix(width);
  assert(x_row_order.data != NULL);
  assert(y_row_order.data != NULL);
  assert(z_row_order.data != NULL);
  assert(x_hybrid_order.data != NULL);
  assert(y_hybrid_order.data != NULL);
  assert(z_hybrid_order.data != NULL);
  int ints_stored = total_ints_stored();
  printf("Seq Pre Ints Stored: %d, expected: %d\n", ints_stored,
         expected_n_ints_stored);
  assert(ints_stored == expected_n_ints_stored &&
         "ERROR: pre Seq did not store expected number of ints");
  zero_storage();
  printf("Completed: test_mm_hybrid setup, mem_usage_limit: %d\n",
         mem_usage_limit);

  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  zero_storage();
  set_int_storage_limit(mem_usage_limit);
  mm_hybrid(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  to_row_order(&z_hybrid_order, &z_row_order);

  assert(total_ints_stored() <= mem_usage_limit &&
         "ERROR: Hybrid Mem Usage Greater than limit");
  printf("Hybrid Mem Usage: %zu\n", total_ints_stored());

  for (int i = 0; i < size; i++) {
    int diff = z_row_order.data[i] - expected_row_order->data[i];
    assert(diff == 0 && "ERROR: Hybrid did not match expected");
  }

  free_matrix(&x_row_order);
  free_matrix(&y_row_order);
  free_matrix(&z_row_order);
  free_matrix(&x_hybrid_order);
  free_matrix(&y_hybrid_order);
  free_matrix(&z_hybrid_order);
  printf("Completed Test: Hybrid, mem_usage_limit: %d\n", mem_usage_limit);
}

void run_test_cases(int base_width, int power) {
  BASE_WIDTH = base_width;
  int prob_width = BASE_WIDTH * pow(2, power);
  int prob_size = prob_width * prob_width;
  printf(
      "*** RUNNING TEST CASES, base_width: %d, power: %d, prob_width: %d, "
      "prob_size: %d ...\n",
      base_width, power, prob_width, prob_size);

  struct SquareMatrix expected_row_order = generate_expected(prob_width);
  test_mm_in(&expected_row_order);
  test_mm_out(&expected_row_order, power);
  test_mm_hybrid(&expected_row_order, 0);
  test_mm_hybrid(&expected_row_order, 512);
  test_mm_hybrid(&expected_row_order, 800);

  free_matrix(&expected_row_order);
  printf(
      "*** COMPLETED TEST CASES, base_width: %d, power: %d , prob_width: %d, "
      "prob_size: %d  ...\n",
      base_width, power, prob_width, prob_size);
}

// Ensure that mm_in, mm_out, and mm_hybrid produce the same result
int main(void) {
  for (int power = 2; power < 8; power++) {
    // run_test_cases(1, power);
    run_test_cases(3, power);
    run_test_cases(5, power);
    run_test_cases(7, power);
  }

  printf("Complete\n");
  return 0;
}
