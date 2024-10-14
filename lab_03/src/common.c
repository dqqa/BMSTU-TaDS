#include "common.h"
#include "errors.h"
#include "sparse_mat_a.h"

static DATA_TYPE calc_mult(const void *mat1, mat_common_t mat1_props, const void *mat2, mat_common_t mat2_props, size_t i, size_t j)
{
    DATA_TYPE res = 0;
    for (size_t ii = 0; ii < mat1_props.m; ii++)
    {
        DATA_TYPE a, b;
        mat1_props.getter(mat1, i, ii, &a);
        mat2_props.getter(mat2, ii, j, &b);

        res += a * b;
    }

    return res;
}

int mat_multiply(const void *mat1, mat_common_t mat1_props, const void *mat2, mat_common_t mat2_props, void *result, mat_common_t result_props)
{
    if (mat1_props.m != mat2_props.n || mat1_props.n != mat2_props.m)
        return ERR_PARAMS;

    for (size_t i = 0; i < mat1_props.n; i++)
    {
        for (size_t j = 0; j < mat2_props.m; j++)
        {
            DATA_TYPE mult = calc_mult(mat1, mat1_props, mat2, mat2_props, i, j);
            result_props.setter(result, i, j, &mult);
        }
    }
    return ERR_OK;
}
