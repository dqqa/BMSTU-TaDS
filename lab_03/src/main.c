#include "errors.h"
#include "mat_csc.h"
#include "mat_csr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/*
TODO:

1. Умножить две матрицы в указанных формах (А и Б). Результат - в форме А.
2. Умножение - стандартный алгоритм.
3. Сравнение времени выполнения операций и объем памяти при использовании этих 2-х алг.
   при различном проценте их заполенения.
*/

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

bool check_if_input_needed(const char *inp)
{
    return strcasecmp(inp, "y") == 0;
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
            printf("Матрица 1:\n");
            mat_csr_print_internal(&mat1);

            printf("\nМатрица 2:\n");
            mat_csc_print_internal(&mat2);
        }
        else if (op == OP_PRINT)
        {
            printf("Матрица 1:\n");
            mat_print(stdout, &mat1, mat1.base);

            printf("\nМатрица 2:\n");
            mat_print(stdout, &mat2, mat2.base);
        }
        else if (op == OP_MULT_SPARSE)
        {
            if (res_created)
            {
                printf("Результирующая матрица:\n");
                mat_print(stdout, &res, res.base);
            }
            else
            {
                if (!(mat1_created && mat2_created))
                    printf("Необходимо загрузить две матрицы!\n");
                else
                {
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
            printf("TODO");
        }
        else if (op == OP_MULT_STD)
        {
            printf("TODO");
        }
        err:
        if (rc != ERR_OK)
            printf("Ошибка!\n");
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
