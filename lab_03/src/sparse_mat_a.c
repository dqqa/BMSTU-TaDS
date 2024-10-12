#include "sparse_mat_a.h"
#include "errors.h"

int mat_a_create(mat_a_t *mat)
{
    /* vtable emulation :) */
    mat->props.getter = mat_a_get_element;
    mat->props.setter = mat_a_set_element;

    return ERR_OK;
}

int mat_a_get_element(const void *mat, size_t i, size_t j, DATA_TYPE *dst)
{
    return ERR_OK;
}

int mat_a_set_element(void *mat, size_t i, size_t j, const DATA_TYPE *src)
{
    return ERR_OK;
}
