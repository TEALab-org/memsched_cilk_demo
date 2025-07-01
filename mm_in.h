#ifndef MM_IN_H
#define MM_IN_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void mm_add(struct SquareMatrix* x, struct SquareMatrix* y);

/// Assume row major order
void mm_seq(struct SquareMatrix* a, struct SquareMatrix* b, struct SquareMatrix* r);

void mm_in(struct SquareMatrix* x, struct SquareMatrix* y, struct SquareMatrix* z);

void mm_out(struct SquareMatrix* x, struct SquareMatrix* y, struct SquareMatrix* z);

#endif
