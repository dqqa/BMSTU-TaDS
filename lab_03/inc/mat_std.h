#ifndef MAT_STD_H__
#define MAT_STD_H__

#include "common.h"

typedef struct __mat_std_t
{
    mat_common_t base;

    DATA_TYPE **rows;
} mat_std_t;

int mat_std_create(mat_std_t *mat, size_t n, size_t m);
void mat_std_free(mat_std_t *mat);

int mat_std_read(FILE *fp, mat_std_t *mat);
int mat_std_read_ex(const char *filename, mat_std_t *mat);

int mat_std_get_element(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
int mat_std_set_element(void *mat, size_t i, size_t j, const DATA_TYPE *src);

size_t std_calc_size(const mat_std_t *mat);
void mat_std_print_internal(const mat_std_t *mat);

#endif // MAT_STD_H__
