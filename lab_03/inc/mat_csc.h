#ifndef MAT_CSC_H__
#define MAT_CSC_H__

#include "common.h"
#include "mat_csr.h"

typedef mat_csr_t mat_csc_t;

int mat_csc_create(mat_csc_t *mat, size_t n, size_t m);
void mat_csc_free(mat_csc_t *mat);

int mat_csc_read(FILE *fp, mat_csc_t *mat);
int mat_csc_read_ex(const char *filename, mat_csc_t *mat);

int mat_csc_get_element(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
int mat_csc_set_element(void *mat, size_t i, size_t j, const DATA_TYPE *src);

void mat_csc_print_internal(const mat_csc_t *mat);

#endif // MAT_CSC_H__
