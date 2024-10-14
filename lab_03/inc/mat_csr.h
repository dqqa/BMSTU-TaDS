#ifndef MAT_CSR_H__
#define MAT_CSR_H__

#include "common.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

typedef struct __mat_csr_t
{
    mat_common_t base;

    size_t data_cnt;
    DATA_TYPE *data;

    size_t col_indices_cnt;
    size_t *col_indices;

    size_t row_ptrs_cnt;
    size_t *row_ptrs;
} mat_csr_t;

int mat_csr_create(mat_csr_t *mat, size_t n, size_t m);
void mat_csr_free(mat_csr_t *mat);

int mat_csr_read(FILE *fp, mat_csr_t *mat);

int mat_csr_get_element(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
int mat_csr_set_element(void *mat, size_t i, size_t j, const DATA_TYPE *src);

void mat_csr_print_internal(const mat_csr_t *mat);

#endif // MAT_CSR_H__
