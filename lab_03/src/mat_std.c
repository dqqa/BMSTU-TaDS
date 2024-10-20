#include "mat_std.h"
#include "common.h"
#include "errors.h"
#include <stdlib.h>

int mat_std_create(mat_std_t *mat, size_t n, size_t m)
{
    mat->base.type = MAT_STD;
    mat->base.getter = mat_std_get_element;
    mat->base.setter = mat_std_set_element;

    mat->base.n = n;
    mat->base.m = m;

    mat->rows = calloc(n, sizeof(*mat->rows));
    if (!mat->rows)
        return ERR_IO;

    for (size_t i = 0; i < n; i++)
    {
        mat->rows[i] = malloc(sizeof(*mat->rows[i]) * m);
        if (!mat->rows[i])
        {
            mat_std_free(mat);
            return ERR_ALLOC;
        }
    }

    return ERR_OK;
}

void mat_std_free(mat_std_t *mat)
{
    if (!mat->rows)
        return;

    for (size_t i = 0; i < mat->base.n; i++)
        free(mat->rows[i]);
    free(mat->rows);
}

int mat_std_read(FILE *fp, mat_std_t *mat)
{
    for (size_t i = 0; i < mat->base.n; i++)
    {
        for (size_t j = 0; j < mat->base.m; j++)
        {
            DATA_TYPE el;
            if (fscanf(fp, "%" DATA_SCN, &el) != 1)
                return ERR_IO;

            int rc;
            if ((rc = mat_std_set_element(mat, i, j, &el) != ERR_OK))
                return rc;
        }
    }
    return ERR_OK;
}

int mat_std_read_ex(const char *filename, mat_std_t *mat)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = ERR_OK;
    size_t n, m;
    if (fscanf(fp, "%zu%zu", &n, &m) != 2)
    {
        rc = ERR_IO;
        goto cleanup;
    }

    if ((rc = mat_std_create(mat, n, m)) != ERR_OK)
        goto cleanup;

    rc = mat_std_read(fp, mat);

    cleanup:
    fclose(fp);

    return rc;
}

int mat_std_get_element(const void *src, size_t i, size_t j, DATA_TYPE *dst)
{
    const mat_std_t *mat = src;
    if (i > mat->base.n || j > mat->base.m)
        return ERR_RANGE;

    *dst = mat->rows[i][j];
    return ERR_OK;
}

int mat_std_set_element(void *dst, size_t i, size_t j, const DATA_TYPE *src)
{
    mat_std_t *mat = dst;
    if (i > mat->base.n || j > mat->base.m)
        return ERR_RANGE;

    mat->rows[i][j] = *src;
    return ERR_OK;
}

void mat_std_print_internal(const mat_std_t *mat);
