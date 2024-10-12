#ifndef SPARSE_MAT_A_H__
#define SPARSE_MAT_A_H__

#include "common.h"
#include <inttypes.h>
#include <stddef.h>

typedef struct __sparse_mat_a
{
    DATA_TYPE *data;
    size_t *rows;
    size_t *ja;
} mat_a_t;

int mat_a_get_element(const mat_a_t *mat, size_t i, size_t j, DATA_TYPE *dst);
int mat_a_set_element(mat_a_t *mat, size_t i, size_t j, const DATA_TYPE *src);

#endif // SPARSE_MAT_A_H__
