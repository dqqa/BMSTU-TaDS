#include "mat_csc.h"
#include "common.h"
#include "errors.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mat_csc_create(mat_csc_t *mat, size_t n, size_t m)
{
    int rc;
    mat->base.type = MAT_CSC;

    /* vtable emulation :) */
    mat->base.getter = mat_csc_get_element;
    mat->base.setter = mat_csc_set_element;

    mat->base.n = n;
    mat->base.m = m;

    mat->data = NULL;
    mat->col_indices = NULL;
    mat->row_ptrs = NULL;

    mat->data = malloc(sizeof(DATA_TYPE));
    mat->data_cnt = 1;
    if (!mat->data)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }

    mat->col_indices = malloc(sizeof(size_t));
    mat->col_indices_cnt = 1;
    if (!mat->col_indices)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }

    mat->row_ptrs = calloc(m + 1, sizeof(size_t));
    mat->row_ptrs_cnt = m + 1;
    if (!mat->row_ptrs)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }

    return ERR_OK;

    cleanup:
    free(mat->data);
    free(mat->row_ptrs);
    free(mat->col_indices);

    return rc;
}

void mat_csc_free(mat_csc_t *mat)
{
    if (mat->data)
        free(mat->data);
    if (mat->col_indices)
        free(mat->col_indices);
    if (mat->row_ptrs)
        free(mat->row_ptrs);
}

int mat_csc_read(FILE *fp, mat_csc_t *mat)
{
    for (size_t i = 0; i < mat->base.n; i++)
    {
        for (size_t j = 0; j < mat->base.m; j++)
        {
            DATA_TYPE el;
            if (fscanf(fp, "%" DATA_SCN, &el) != 1)
                return ERR_IO;

            int rc;
            if ((rc = mat_csc_set_element(mat, i, j, &el) != ERR_OK))
                return rc;
        }
    }
    return ERR_OK;
}

int mat_csc_get_element(const void *src, size_t i, size_t j, DATA_TYPE *dst)
{
    const mat_csr_t *mat = src;

    if (i >= mat->base.n || j >= mat->base.m)
        return ERR_RANGE;

    if (!mat->data_cnt)
        return ERR_NOT_FOUND;

    size_t cur_row = mat->row_ptrs[j];
    size_t next_row = mat->row_ptrs[j + 1];
    bool found = false;

    for (size_t jj = 0; !found && jj < next_row - cur_row; jj++)
    {
        if (mat->col_indices[cur_row + jj] == i)
        {
            found = true;
            *dst = mat->data[cur_row + jj];
        }
    }

    if (!found)
    {
        *dst = 0;
        return ERR_NOT_FOUND;
    }
    return ERR_OK;
}

static size_t get_min_ind(size_t *arr, size_t size, size_t val)
{
    for (size_t i = size; i-- > 0;)
        if (arr[i] < val)
            return i + 1;

    return 0;
}

int mat_csc_set_element(void *dst, size_t i, size_t j, const DATA_TYPE *src)
{
    mat_csr_t *mat = dst;
    if (i >= mat->base.n || j >= mat->base.m)
        return ERR_RANGE;

    size_t cur_row = mat->row_ptrs[j];
    size_t next_row = mat->row_ptrs[j + 1];
    size_t nz_el_count = next_row - cur_row;

    DATA_TYPE tmp;
    int rc = mat_csc_get_element(dst, i, j, &tmp);
    if (rc == ERR_OK) // if element in mat[i][j] != 0
    {
        if (!DATA_CMP_EQ(*src, 0))
        {
            for (size_t jj = 0; jj < nz_el_count; jj++)
            {
                if (mat->col_indices[cur_row + jj] == i)
                {
                    mat->data[mat->col_indices[cur_row + jj]] = *src;
                    return ERR_OK;
                }
            }
        }
        else
        {
            // shrink
            size_t ind = 0;
            for (size_t j = 0; j < next_row - cur_row; j++)
            {
                if (mat->col_indices[cur_row + j] == i)
                {
                    ind = j;
                    break;
                }
            }

            memmove(mat->col_indices + cur_row + ind, mat->col_indices + cur_row + ind + 1, sizeof(size_t) * (mat->col_indices_cnt - cur_row - ind));
            memmove(mat->data + cur_row + ind, mat->data + cur_row + ind + 1, sizeof(DATA_TYPE) * (mat->data_cnt - cur_row - ind));

            DATA_TYPE *tmp_data = realloc(mat->data, sizeof(*mat->data) * (mat->data_cnt - 1));
            if (!tmp_data)
                return ERR_ALLOC;
            mat->data = tmp_data;

            size_t *tmp_col_indices = realloc(mat->col_indices, sizeof(*mat->col_indices) * (mat->col_indices_cnt - 1));
            if (!tmp_col_indices)
                return ERR_ALLOC;
            mat->col_indices = tmp_col_indices;

            mat->data_cnt--;
            mat->col_indices_cnt--;

            // decrement
            for (size_t jj = i + 1; jj < mat->row_ptrs_cnt; jj++)
                mat->row_ptrs[jj]--;
        }
    }

    if (*src == 0)
        return ERR_OK;

    // if element in mat[i][j] == 0 && src != 0
    // then we should increment row_ptrs after i-th with 1
    // then insert into data array and col_indicies array following data

    size_t new_col = get_min_ind(mat->col_indices + cur_row, nz_el_count, i);

    DATA_TYPE *tmp_data = realloc(mat->data, sizeof(*mat->data) * (mat->data_cnt + 1));
    if (!tmp_data)
        return ERR_ALLOC;
    mat->data = tmp_data;

    size_t *tmp_col_indices = realloc(mat->col_indices, sizeof(*mat->col_indices) * (mat->col_indices_cnt + 1));
    if (!tmp_col_indices)
        return ERR_ALLOC;
    mat->col_indices = tmp_col_indices;

    memmove(mat->col_indices + cur_row + new_col + 1, mat->col_indices + cur_row + new_col, sizeof(size_t) * (mat->col_indices_cnt - cur_row - new_col));
    memmove(mat->data + cur_row + new_col + 1, mat->data + cur_row + new_col, sizeof(DATA_TYPE) * (mat->data_cnt - new_col - cur_row));

    mat->col_indices[cur_row + new_col] = i;
    mat->col_indices_cnt++;

    mat->data[cur_row + new_col] = *src;
    mat->data_cnt++;

    for (size_t jj = j + 1; jj < mat->row_ptrs_cnt; jj++)
        mat->row_ptrs[jj]++;

    return ERR_OK;
}

static size_t csc_calc_size(const mat_csc_t *mat)
{
    size_t result = 0;
    result += sizeof(mat->base);
    result += mat->data_cnt * sizeof(*mat->data);
    result += mat->row_ptrs_cnt * sizeof(*mat->row_ptrs);
    result += mat->col_indices_cnt * sizeof(*mat->col_indices);

    return result;
}

void mat_csc_print_internal(const mat_csc_t *mat)
{
    printf("Массив значений:            {");
    for (size_t i = 0; i < mat->data_cnt - 1; i++)
        printf(i == mat->data_cnt - 2 ? "%" DATA_PRI : "%" DATA_PRI ", ", mat->data[i]);
    printf("}\n");

    printf("Индексы строк:              {");
    for (size_t i = 0; i < mat->col_indices_cnt - 1; i++)
        printf(i == mat->col_indices_cnt - 2 ? "%zu" : "%zu, ", mat->col_indices[i]);
    printf("}\n");

    printf("Массив индексации столбцов: {");
    for (size_t i = 0; i < mat->row_ptrs_cnt; i++)
        printf(i == mat->row_ptrs_cnt - 1 ? "%zu" : "%zu, ", mat->row_ptrs[i]);
    printf("}\n");

    printf("Размер структуры CSC: %zu байт\n", csc_calc_size(mat));
}

/**
 * @brief Выделить память и заполнить матрицу CSC
 *
 * @param filename Путь
 * @param mat Указатель на матрицу в формате CSC
 * @return int код ошибки
 */
int mat_csc_read_ex(const char *filename, mat_csc_t *mat)
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

    if ((rc = mat_csc_create(mat, n, m)) != ERR_OK)
        goto cleanup;

    rc = mat_csc_read(fp, mat);

    cleanup:
    fclose(fp);
    return rc;
}
