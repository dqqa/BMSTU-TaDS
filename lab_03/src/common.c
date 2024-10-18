#include "common.h"
#include "errors.h"
#include "mat_csc.h"
#include "mat_csr.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int mat_print(FILE *fp, const void *mat, mat_common_t mat_props)
{
    for (size_t i = 0; i < mat_props.n; i++)
    {
        for (size_t j = 0; j < mat_props.m; j++)
        {
            DATA_TYPE el;
            mat_props.getter(mat, i, j, &el);
            if (fprintf(fp, "%3" DATA_PRI " ", el) < 0)
                return ERR_IO;
        }
        fprintf(fp, "\n");
    }
    return ERR_OK;
}

void mat_fill_rnd(void *mat, mat_common_t mat_props, int percent)
{
    srand(time(NULL));
    for (size_t i = 0; i < mat_props.n; i++)
    {
        for (size_t j = 0; j < mat_props.m; j++)
        {
            DATA_TYPE val = (rand() % 100) < percent ? rand() % 10 : 0;
            mat_props.setter(mat, i, j, &val);
        }
    }
}

int mat_fill_rnd_ex(void *mat, mat_type_t t)
{
    int rc = ERR_OK;
    size_t n, m;

    printf("Введите количество строк и столбцов: ");
    if (scanf("%zu%zu", &n, &m) != 2)
        return ERR_IO;

    if (t == MAT_CSR)
        rc = mat_csr_create(mat, n, m);
    else if (t == MAT_CSC)
        rc = mat_csc_create(mat, n, m);

    if (rc != ERR_OK)
        return rc;

    int p;
    printf("Введите процент заполнения: ");

    if (scanf("%d", &p) != 1)
        return ERR_IO;

    mat_fill_rnd(mat, ((mat_csr_t *)mat)->base, p);

    return ERR_OK;
}

int mat_read_ex(const char *filename, void *mat, mat_type_t t)
{
    FILE *fp = NULL;
    if (!(fp = fopen(filename, "r")))
        return ERR_IO;

    int rc = ERR_OK;
    if (t == MAT_CSR)
        rc = mat_csr_read_ex(filename, mat);
    else if (t == MAT_CSC)
        rc = mat_csc_read_ex(filename, mat);

    if (rc != ERR_OK)
        goto cleanup;

    cleanup:
    fclose(fp);
    return rc;
}

static int str_input(char *buf, size_t buf_size)
{
    if (!fgets(buf, buf_size, stdin))
        return ERR_IO;

    char *newline = strchr(buf, '\n');
    if (newline)
        *newline = 0;
    else
        return ERR_OVERFLOW;

    return ERR_OK;
}

static load_t get_file_op(void)
{
    printf("1. Из файла\n"
           "2. Случайно\n");
    printf("Введите номер операции: ");

    int input;
    int rc = scanf("%d", &input);
    getchar();
    if (rc < 0)
        return LOAD_EOF;
    if (rc == 0 || input > LOAD_COUNT || input < 1)
        return LOAD_UNKNOWN;

    return (load_t)(input - 1);
}

int mat_fill_ex(void *mat, mat_type_t t)
{
    int rc = ERR_OK;
    char buf[128];

    load_t l = get_file_op();
    if (l == LOAD_FROM_FILE)
    {
        printf("Введите путь до файла: ");
        rc = str_input(buf, sizeof(buf));
        if (rc == ERR_OK)
            rc = mat_read_ex(buf, mat, t);
    }
    else if (l == LOAD_RAND)
    {
        rc = mat_fill_rnd_ex(mat, t);
    }

    return rc;
}
