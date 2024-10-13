#include "sparse_mat_a.h"
#include "errors.h"
#include <stdbool.h>
#include <stdlib.h>

int mat_a_create(mat_a_t *mat, size_t n, size_t m)
{
    /* vtable emulation :) */
    mat->props.getter = mat_a_get_element;
    mat->props.setter = mat_a_set_element;

    mat->props.n = n;
    mat->props.m = m;

    return ERR_OK;
}

// TODO: check this
int mat_a_get_element(const void *src, size_t i, size_t j, DATA_TYPE *dst)
{
    const mat_a_t *mat = src;

    size_t cur_row = mat->row_ptrs[i];
    size_t next_row = mat->row_ptrs[i + 1]; // overflow? add memory border checks
    bool found = false;

    for (size_t ii = 0; !found && ii < next_row - cur_row; ii++)
    {
        if (mat->col_indeices[cur_row + ii] == j)
        {
            found = true;
            *dst = mat->data[mat->col_indeices[cur_row + ii]];
        }
    }

    if (!found)
        *dst = 0;

    return ERR_OK;
}

int mat_a_set_element(void *dst, size_t i, size_t j, const DATA_TYPE *src)
{
    mat_a_t *mat = dst;
    if (i >= mat->props.n || j >= mat->props.m)
        return ERR_RANGE;

    return ERR_OK;
}

int mat_a_read(FILE *fp, size_t n, size_t m, mat_a_t *mat)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            DATA_TYPE el;
            if (fscanf("%" DATA_SCN, &el) != 1)
                return ERR_IO;

            int rc;
            if ((rc = mat_a_set_element(mat, i, j, &el) != ERR_OK))
                return rc;
        }
    }
    return ERR_OK;
}

int mat_a_save(FILE *fp, size_t n, size_t m, const mat_a_t *mat);
