#include "errors.h"
#include "mat_csc.h"
#include "mat_csr.h"
#include "mat_std.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>

/*
TODO:

1. Умножить две матрицы в указанных формах (А и Б). Результат - в форме А.
2. Умножение - стандартный алгоритм.
3. Сравнение времени выполнения операций и объем памяти при использовании этих 2-х алг.
   при различном проценте их заполенения.
*/

#define PERF_CSV 1
#define PERF_REPEATS 5

typedef enum
{
    OP_EXIT,
    OP_LOAD,
    OP_PRINT,
    OP_PRINT_INTERNAL,
    OP_MULT_STD,
    OP_MULT_SPARSE,
    OP_EDIT,
    OP_PERF,
    OP_COUNT,
    OP_UNKNOWN,
    OP_EOF
} op_t;

op_t get_op(void)
{
    printf("\n");
    printf("0. Выйти\n"
           "1. Заполнить матрицы\n"
           "2. Вывести матрицы\n"
           "3. Вывести внутреннее представление матриц\n"
           "4. Умножить матрицы (массив массивов)\n"
           "5. Умножить матрицы (разреженные)\n"
           "6. Редактировать матрицу\n"
           "7. Сравнить эффективность\n");
    printf("Введите номер операции: ");
    int input;
    int rc = scanf("%d", &input);
    getchar();
    if (rc < 0)
        return OP_EOF;
    if (rc == 0 || input >= OP_COUNT || input < 0)
        return OP_UNKNOWN;

    return (op_t)input;
}

void print_usage(void)
{
    fprintf(stderr, "Usage: app.exe\n");
}

void print_guide(void)
{
    printf("Программа для умножения матриц в разных формах: CSR и CSC. Вывод в формате CSR.\n"
           "На вход программе подается 2 файла с матрицами:\n"
           "  - На первой строке содержатся два числа:\n"
           "    - Кол-во строк\n"
           "    - Кол-во столбцов\n"
           "  - На последующих строках файла содержится целочисленная матрица\n\n");
}

static int mat_cpy(void *dst, mat_common_t dst_props, const void *src, mat_common_t src_props)
{
    if (dst_props.n != src_props.n || dst_props.m != src_props.m)
        return ERR_PARAMS;

    for (size_t i = 0; i < src_props.n; i++)
    {
        for (size_t j = 0; j < src_props.m; j++)
        {
            DATA_TYPE el;
            src_props.getter(src, i, j, &el);
            dst_props.setter(dst, i, j, &el);
        }
    }
    return ERR_OK;
}

int performance_comparison(void)
{
    struct timespec start, end;
    mat_csr_t csr_mat1 = {0}, csr_mat2 = {0}, csr_res = {0};
    mat_std_t std_mat1 = {0}, std_mat2 = {0}, std_res = {0};
    int rc = ERR_OK;
#if PERF_CSV
    printf("percent;size;csr_size;std_size;size_frac;csr_time;std_time;time_frac\n");
#endif
    for (size_t p = 10; rc == ERR_OK && p <= 100; p += 20)
    {
        for (size_t s = 10; rc == ERR_OK && s <= 100; s += 10)
        {
            if ((rc = mat_csr_create(&csr_mat1, s, s)) != ERR_OK)
                goto cleanup;
            if ((rc = mat_csr_create(&csr_mat2, s, s)) != ERR_OK)
                goto cleanup;
            if ((rc = mat_csr_create(&csr_res, s, s)) != ERR_OK)
                goto cleanup;

            if ((rc = mat_std_create(&std_mat1, s, s)) != ERR_OK)
                goto cleanup;
            if ((rc = mat_std_create(&std_mat2, s, s)) != ERR_OK)
                goto cleanup;
            if ((rc = mat_std_create(&std_res, s, s)) != ERR_OK)
                goto cleanup;

            mat_fill_rnd(&csr_mat1, csr_mat1.base, p);
            mat_fill_rnd(&csr_mat2, csr_mat2.base, p);
#if !PERF_CSV
            printf("Размер: %zux%zu. Заполнение: %zu%%.\n", s, s, p);
#endif
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            // for (size_t i = 0; i < PERF_REPEATS; i++) // without mat_free() memory errors!
            mat_multiply(&csr_mat1, csr_mat1.base, &csr_mat2, csr_mat2.base, &csr_res, csr_res.base);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            double t1 = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) / 2000;
            // t /= PERF_REPEATS;

            mat_cpy(&std_mat1, std_mat1.base, &csr_mat1, csr_mat1.base);
            mat_cpy(&std_mat2, std_mat2.base, &csr_mat2, csr_mat2.base);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            // for (size_t i = 0; i < PERF_REPEATS; i++) // without mat_free() memory errors!
            mat_multiply(&std_mat1, std_mat1.base, &std_mat2, std_mat2.base, &std_res, std_res.base);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            double t2 = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) / 1000;
            // t /= PERF_REPEATS;

            float csr_size = csr_calc_size(&csr_mat1);
            float std_size = std_calc_size(&std_mat1);
#if !PERF_CSV
            printf("  Размер CSR=%10.0f, STD=%10.0f. Эффективность: %+.2f%%\n", csr_size, std_size, (std_size - csr_size) / std_size * 100);
            printf("  Время  CSR=%10.2f, STD=%10.2f. Эффективность: %+.2f%%\n", t1, t2, (t2 - t1) / t2 * 100);
#else
            printf("%zu;%zu;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f\n", p, s, csr_size, std_size, (std_size - csr_size) / std_size * 100, t1, t2, (t2 - t1) / t2 * 100);
#endif
            cleanup:
            mat_csr_free(&csr_mat1);
            mat_csr_free(&csr_mat2);
            mat_csr_free(&csr_res);

            mat_std_free(&std_mat1);
            mat_std_free(&std_mat2);
            mat_std_free(&std_res);
        }
    }

    return rc;
}

int main(void)
{
    print_guide();

    FILE *fp1 = NULL, *fp2 = NULL;

    bool mat1_created = false, mat2_created = false, res_created = false;
    bool need_exit = false;
    mat_csr_t mat1 = {0}, res = {0};
    mat_csc_t mat2 = {0};

    int rc = ERR_OK;
    while (!need_exit)
    {
        op_t op = get_op();
        if (op == OP_EXIT || op == OP_EOF)
        {
            need_exit = true;
            break;
        }
        else if (op == OP_LOAD)
        {
            if (!mat1_created)
            {
                rc = mat_fill_ex(&mat1, MAT_CSR);
                if (rc == ERR_OK)
                    mat1_created = true;
            }
            else
                printf("Матрица 1 уже загружена!\n");

            if (!mat2_created)
            {
                rc = mat_fill_ex(&mat2, MAT_CSC);
                if (rc == ERR_OK)
                    mat2_created = true;
            }
            else
                printf("Матрица 2 уже загружена!\n");
        }
        else if (op == OP_PRINT_INTERNAL)
        {
            if (mat1.data)
            {
                printf("Матрица 1:\n");
                mat_csr_print_internal(&mat1);
            }

            if (mat2.data)
            {
                printf("\nМатрица 2:\n");
                mat_csc_print_internal(&mat2);
            }
        }
        else if (op == OP_PRINT)
        {
            if (mat1.data)
            {
                printf("Матрица 1:\n");
                mat_print(stdout, &mat1, mat1.base);
            }

            if (mat2.data)
            {
                printf("\nМатрица 2:\n");
                mat_print(stdout, &mat2, mat2.base);
            }
        }
        else if (op == OP_MULT_SPARSE)
        {
            // if (res_created)
            // {
            //     printf("Результирующая матрица:\n");
            //     mat_print(stdout, &res, res.base);
            // }
            // else
            {
                if (!(mat1_created && mat2_created))
                    printf("Необходимо загрузить две матрицы!\n");
                else
                {
                    mat_csr_free(&res);
                    rc = mat_csr_create(&res, mat1.base.m, mat2.base.n);
                    if (rc == ERR_OK)
                        rc = mat_multiply(&mat1, mat1.base, &mat2, mat2.base, &res, res.base);
                    else
                        printf("Невозможно создать матрицу\n");
                    if (rc == ERR_OK)
                    {
                        printf("Результирующая матрица:\n");
                        mat_print(stdout, &res, res.base);
                        res_created = true;
                    }
                    else
                        printf("Невозможно умножить эти две матрицы\n");
                }
            }
        }
        else if (op == OP_EDIT)
        {
            int mat_id;
            printf("Введите номер матрицы: ");
            if (scanf("%d", &mat_id) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            if (mat_id > 2 || mat_id < 1)
            {
                rc = ERR_RANGE;
                goto err;
            }
            if ((mat_id == 1 && !mat1_created) || (mat_id == 2 && !mat2_created))
            {
                rc = ERR_RANGE;
                goto err;
            }

            bool need_insert = true;
            while (need_insert)
            {
                printf("Введите строку и столбец: ");
                size_t i, j;
                if (scanf("%zu%zu", &i, &j) == 2)
                {
                    if (i < 1 || j < 1)
                    {
                        rc = ERR_RANGE;
                        goto err;
                    }

                    i--;
                    j--;

                    DATA_TYPE el;
                    printf("Введите значение: ");
                    if (scanf("%" DATA_SCN, &el) == 1)
                    {
                        if (mat_id == 1)
                            rc = mat_csr_set_element(&mat1, i, j, &el);
                        else if (mat_id == 2)
                            rc = mat_csc_set_element(&mat2, i, j, &el);
                        else
                            rc = ERR_RANGE;
                        getchar();
                    }
                }

                char inp[32];
                printf("Изменить еще? (y, [n]): ");
                str_input(inp, sizeof(inp));
                need_insert = strcasecmp(inp, "y") == 0;
            }
        }
        else if (op == OP_PERF)
        {
            rc = performance_comparison();
        }
        else if (op == OP_MULT_STD)
        {
            char buf[128];
            mat_std_t mat1 = {0}, mat2 = {0}, res = {0};
            printf("Введите путь до файла с 1-й матрицей: ");
            str_input(buf, sizeof(buf));
            if ((rc = mat_std_read_ex(buf, &mat1)) != ERR_OK)
                goto cleanup;

            printf("Введите путь до файла со 2-й матрицей: ");
            str_input(buf, sizeof(buf));
            if ((rc = mat_std_read_ex(buf, &mat2)) != ERR_OK)
                goto cleanup;

            if ((rc = mat_std_create(&res, mat1.base.m, mat2.base.n)) != ERR_OK)
                goto cleanup;

            if ((rc = mat_multiply(&mat1, mat1.base, &mat2, mat2.base, &res, res.base)) != ERR_OK)
                goto cleanup;

            printf("Результирующая матрица:\n");
            mat_print(stdout, &res, res.base);

            cleanup:
            mat_std_free(&mat1);
            mat_std_free(&mat2);
            mat_std_free(&res);
        }
        err:
        if (rc != ERR_OK)
            printf("Ошибка!\n");
        rc = ERR_OK;
    }

    if (fp1)
        fclose(fp1);
    if (fp2)
        fclose(fp2);

    mat_csr_free(&mat1);
    mat_csc_free(&mat2);
    mat_csr_free(&res);

    return 0;
}
