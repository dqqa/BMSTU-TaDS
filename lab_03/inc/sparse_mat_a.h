#ifndef SPARSE_MAT_A_H__
#define SPARSE_MAT_A_H__

#include "common.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

typedef struct __sparse_mat_a
{
    mat_common_t props;

    size_t data_count;
    DATA_TYPE *data;
    size_t *col_indeices;
    size_t *row_ptrs;
} mat_a_t;

int mat_a_create(mat_a_t *mat, size_t n, size_t m);
int mat_a_free(mat_a_t *mat);

int mat_a_read(FILE *fp, size_t n, size_t m, mat_a_t *mat);
int mat_a_save(FILE *fp, size_t n, size_t m, const mat_a_t *mat);

int mat_a_get_element(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
int mat_a_set_element(void *mat, size_t i, size_t j, const DATA_TYPE *src);

#endif // SPARSE_MAT_A_H__
