#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "square_matrix.h"

// Add y to x
void mm_add(struct SquareMatrix* x, struct SquareMatrix* y);

// Sequential matrix multiplication
// Assume row major order
void mm_seq(struct SquareMatrix* a,
            struct SquareMatrix* b,
            struct SquareMatrix* r);

// In place recursive matrix multiplication
// Assume hybrid major order
void mm_in(struct SquareMatrix* x,
           struct SquareMatrix* y,
           struct SquareMatrix* z);

// Out-of-place recursive matrix multiplication
// Assume hybrid major order
void mm_out(struct SquareMatrix* x,
            struct SquareMatrix* y,
            struct SquareMatrix* z);

// Hybrid recursive matrix multiplication
// Assume hybrid major order
void mm_hybrid(struct SquareMatrix* x,
               struct SquareMatrix* y,
               struct SquareMatrix* z);

#endif
