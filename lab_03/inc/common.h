#ifndef COMMON_H__
#define COMMON_H__

#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

typedef int32_t DATA_TYPE;
#define DATA_PRI PRId32
#define DATA_SCN SCNd32
#define DATA_CMP_EQ(a, b) ((a) == (b))

typedef enum
{
    LOAD_FROM_FILE,
    LOAD_RAND,
    LOAD_CONSOLE,
    LOAD_COUNT,
    LOAD_UNKNOWN,
    LOAD_EOF
} load_t;

typedef int (*mat_getter_t)(const void *mat, size_t i, size_t j, DATA_TYPE *dst);
typedef int (*mat_setter_t)(void *mat, size_t i, size_t j, const DATA_TYPE *src);

typedef enum
{
    MAT_CSR,
    MAT_CSC,
    MAT_STD
} mat_type_t;

typedef struct __mat_common
{
    size_t n, m;
    mat_getter_t getter;
    mat_setter_t setter;
    mat_type_t type;
} mat_common_t;

int mat_multiply(const void *mat1, mat_common_t mat1_props, const void *mat2, mat_common_t mat2_props, void *result, mat_common_t result_props);
int mat_print(FILE *fp, const void *mat, mat_common_t mat_props);

int mat_read_ex(const char *filename, void *mat, mat_type_t t);

void mat_fill_rnd(void *mat, mat_common_t mat_props, int percent);
int mat_fill_rnd_ex(void *mat, mat_type_t t);

int mat_fill_ex(void *mat, mat_type_t t);

int str_input(char *buf, size_t buf_size);

#endif // COMMON_H__
