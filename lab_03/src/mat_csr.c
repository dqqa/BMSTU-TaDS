#include "mat_csr.h"
#include "errors.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int mat_csr_create(mat_csr_t *mat, size_t n, size_t m)
{
    int rc;
    mat->base.type = MAT_CSR;

    /* vtable emulation :) */
    mat->base.getter = mat_csr_get_element;
    mat->base.setter = mat_csr_set_element;

    mat->base.n = n;
    mat->base.m = m;

    mat->data = NULL;
    mat->col_indices = NULL;
    mat->row_ptrs = NULL;

    mat->data = calloc(1, sizeof(DATA_TYPE));
    mat->data_cnt = 1;
    if (!mat->data)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }

    mat->col_indices = calloc(1, sizeof(size_t));
    mat->col_indices_cnt = 1;
    if (!mat->col_indices)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }

    mat->row_ptrs = calloc(n + 1, sizeof(size_t));
    mat->row_ptrs_cnt = n + 1;
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

void mat_csr_free(mat_csr_t *mat)
{
    if (mat->data)
        free(mat->data);
    if (mat->col_indices)
        free(mat->col_indices);
    if (mat->row_ptrs)
        free(mat->row_ptrs);

    memset(mat, 0, sizeof(*mat));
}

int mat_csr_get_element(const void *src, size_t i, size_t j, DATA_TYPE *dst)
{
    const mat_csr_t *mat = src;

    if (i >= mat->base.n || j >= mat->base.m)
        return ERR_RANGE;

    if (!mat->data_cnt)
        return ERR_NOT_FOUND;

    size_t cur_row = mat->row_ptrs[i];
    size_t next_row = mat->row_ptrs[i + 1]; // overflow? add memory border checks
    bool found = false;

    for (size_t ii = 0; !found && ii < next_row - cur_row; ii++)
    {
        if (mat->col_indices[cur_row + ii] == j)
        {
            found = true;
            *dst = mat->data[cur_row + ii];
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

/*
4 случая:
  1. mat[i][j] != 0 && src != 0 // заменяем число из массива данных на новое
  2. mat[i][j] == 0 && src != 0 // увеличиваем размеры массивов, инкрементируем каждый элемент в массиве указателей после i-го(?)
  3. mat[i][j] != 0 && src == 0 // обрезаем массивы уменьшаем в массиве указателей каждый после i(?) на 1
  4. mat[i][j] == 0 && src == 0 // игнорируем
*/

int mat_csr_set_element(void *dst, size_t i, size_t j, const DATA_TYPE *src)
{
    mat_csr_t *mat = dst;
    if (i >= mat->base.n || j >= mat->base.m)
        return ERR_RANGE;

    size_t cur_row = mat->row_ptrs[i];
    size_t next_row = mat->row_ptrs[i + 1];
    size_t nz_el_count = next_row - cur_row;

    DATA_TYPE tmp;
    int rc = mat_csr_get_element(dst, i, j, &tmp);
    if (rc == ERR_OK) // if element in mat[i][j] != 0
    {
        if (!DATA_CMP_EQ(*src, 0))
        {
            for (size_t ii = 0; ii < nz_el_count; ii++)
            {
                if (mat->col_indices[cur_row + ii] == j)
                {
                    mat->data[mat->col_indices[cur_row + ii]] = *src;
                    return ERR_OK;
                }
            }
        }
        else
        {
            // shrink
            size_t ind = 0;
            for (size_t i = 0; i < next_row - cur_row; i++)
            {
                if (mat->col_indices[cur_row + i] == j)
                {
                    ind = i;
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
            for (size_t ii = i + 1; ii < mat->row_ptrs_cnt; ii++)
                mat->row_ptrs[ii]--;
        }
    }

    if (*src == 0)
        return ERR_OK;

    // if element in mat[i][j] == 0
    // then we should increment row_ptrs after i-th with 1
    // then insert into data array and col_indicies array following data

    size_t new_col = get_min_ind(mat->col_indices + cur_row, nz_el_count, j);

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

    mat->col_indices[cur_row + new_col] = j;
    mat->col_indices_cnt++;

    mat->data[cur_row + new_col] = *src;
    mat->data_cnt++;

    for (size_t ii = i + 1; ii < mat->row_ptrs_cnt; ii++)
        mat->row_ptrs[ii]++;

    return ERR_OK;
}

int mat_csr_read(FILE *fp, mat_csr_t *mat)
{
    for (size_t i = 0; i < mat->base.n; i++)
    {
        for (size_t j = 0; j < mat->base.m; j++)
        {
            DATA_TYPE el;
            if (fscanf(fp, "%" DATA_SCN, &el) != 1)
                return ERR_IO;

            int rc;
            if ((rc = mat_csr_set_element(mat, i, j, &el) != ERR_OK))
                return rc;
        }
    }
    return ERR_OK;
}

size_t csr_calc_size(const mat_csr_t *mat)
{
    size_t result = 0;
    result += sizeof(mat->base.n) * 2;
    result += mat->data_cnt * sizeof(*mat->data);
    result += mat->row_ptrs_cnt * sizeof(*mat->row_ptrs);
    result += mat->col_indices_cnt * sizeof(*mat->col_indices);

    return result;
}

void mat_csr_print_internal(const mat_csr_t *mat)
{
    printf("Массив значений:         {");
    for (size_t i = 0; i < mat->data_cnt - 1; i++)
        printf(i == mat->data_cnt - 2 ? "%" DATA_PRI : "%" DATA_PRI ", ", mat->data[i]);
    printf("}\n");

    printf("Индексы столбцов:        {");
    for (size_t i = 0; i < mat->col_indices_cnt - 1; i++)
        printf(i == mat->col_indices_cnt - 2 ? "%zu" : "%zu, ", mat->col_indices[i]);
    printf("}\n");

    printf("Массив индексации строк: {");
    for (size_t i = 0; i < mat->row_ptrs_cnt; i++)
        printf(i == mat->row_ptrs_cnt - 1 ? "%zu" : "%zu, ", mat->row_ptrs[i]);
    printf("}\n");

    printf("Размер структуры CSR: %zu байт\n", csr_calc_size(mat));
}

/**
 * @brief Выделить память и заполнить матрицу CSR
 *
 * @param filename Путь
 * @param mat Указатель на матрицу в формате CSR
 * @return int код ошибки
 */
int mat_csr_read_ex(const char *filename, mat_csr_t *mat)
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

    if ((rc = mat_csr_create(mat, n, m)) != ERR_OK)
        goto cleanup;

    rc = mat_csr_read(fp, mat);

    cleanup:
    fclose(fp);
    return rc;
}
