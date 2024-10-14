#ifndef COMMON_H__
#define COMMON_H__

#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define DATA_TYPE int32_t
#define DATA_PRI PRId32
#define DATA_SCN SCNd32
#define DATA_CMP_EQ(a, b) ((a) == (b))

typedef int (*mat_getter_t)(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
typedef int (*mat_setter_t)(void *mat, size_t i, size_t j, const DATA_TYPE *src);

typedef struct __mat_common
{
    size_t n, m;
    mat_getter_t getter;
    mat_setter_t setter;
} mat_common_t;

int mat_multiply(const void *mat1, mat_common_t mat1_props, const void *mat2, mat_common_t mat2_props, void *result, mat_common_t result_props);
int mat_print(FILE *fp, const void *mat, mat_common_t mat_props);

#endif // COMMON_H__
