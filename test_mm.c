#include "allocator.h"
#include "matrix_ops.c"
#include "matrix_ops.h"
#include "square_matrix.c"
#include "square_matrix.h"
#include <assert.h>

// Sets the values of x and y as each elements linear index (i.e. y_00 = |x|),
// in row order order
void ic(struct SquareMatrix* x, struct SquareMatrix* y) {
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
  int size = width * width; 
  struct SquareMatrix x_row_order = allocate_matrix(width); 
  struct SquareMatrix y_row_order = allocate_matrix(width); 
  struct SquareMatrix z_row_order = allocate_matrix(width); 
  ic(&x_row_order, &y_row_order);
  zero_total_mem();
  mm_seq(&x_row_order, &y_row_order, &z_row_order);
  assert(get_total_mem() == 0 && "ERROR: Seq Mem Usage Not Zero");
  free_matrix(&x_row_order);
  free_matrix(&y_row_order);
  return z_row_order;
}

void test_mm_in(struct SquareMatrix* expected_row_order) {
  int width = expected_row_order->width;
  int size = width * width; 
  struct SquareMatrix x_row_order = allocate_matrix(width); 
  struct SquareMatrix y_row_order = allocate_matrix(width); 
  struct SquareMatrix z_row_order = allocate_matrix(width); 
  struct SquareMatrix x_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix y_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix z_hybrid_order = allocate_matrix(width); 

  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  zero_total_mem();
  mm_in(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  assert(get_total_mem() == 0 && "ERROR: In-place Mem Usage Not Zero");
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

void test_mm_out(struct SquareMatrix* expected_row_order) {
  int width = expected_row_order->width;
  int size = width * width; 
  struct SquareMatrix x_row_order = allocate_matrix(width); 
  struct SquareMatrix y_row_order = allocate_matrix(width); 
  struct SquareMatrix z_row_order = allocate_matrix(width); 
  struct SquareMatrix x_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix y_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix z_hybrid_order = allocate_matrix(width); 

  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  zero_total_mem();
  mm_out(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  to_row_order(&z_hybrid_order, &z_row_order);

  assert(get_total_mem() != 0 && "ERROR: Out-of-place Mem Usage is Zero");
  printf("Out-of-place Mem Usage: %d\n", get_total_mem());

  for (int i = 0; i < size; i++) {
    int diff = z_row_order.data[i] - expected_row_order->data[i];
    printf("%d\n", diff);
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

void test_mm_hybrid(struct SquareMatrix* expected_row_order, int mem_usage_limit) {
  int width = expected_row_order->width;
  int size = width * width; 
  struct SquareMatrix x_row_order = allocate_matrix(width); 
  struct SquareMatrix y_row_order = allocate_matrix(width); 
  struct SquareMatrix z_row_order = allocate_matrix(width); 
  struct SquareMatrix x_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix y_hybrid_order = allocate_matrix(width); 
  struct SquareMatrix z_hybrid_order = allocate_matrix(width); 

  ic(&x_row_order, &y_row_order);
  to_hybrid_order(&x_row_order, &x_hybrid_order);
  to_hybrid_order(&y_row_order, &y_hybrid_order);
  zero_total_mem();
  set_mem_usage_limit(mem_usage_limit);
  mm_hybrid(&x_hybrid_order, &y_hybrid_order, &z_hybrid_order);
  to_row_order(&z_hybrid_order, &z_row_order);
  
  assert(get_total_mem() <= mem_usage_limit && "ERROR: Hybrid Mem Usage Greater than limit");
  printf("Hybrid Mem Usage: %d\n", get_total_mem());

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
  printf("Completed Test: Hybrid\n");
}

// Ensure that mm_in, mm_out, and mm_hybrid produce the same result
int main(void) {
  int prob_width = BASE_WIDTH * 4;

  struct SquareMatrix expected_row_order = generate_expected(prob_width);
  test_mm_in(&expected_row_order);
  test_mm_out(&expected_row_order);
  test_mm_hybrid(&expected_row_order, 0);
  test_mm_hybrid(&expected_row_order, 2000);
  test_mm_hybrid(&expected_row_order, 4000);

  free_matrix(&expected_row_order);
  printf("Complete\n");
  return 0;
}
