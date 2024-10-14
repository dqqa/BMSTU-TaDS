#include "errors.h"
#include "mat_csc.h"
#include "mat_csr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
TODO:

1. Умножить две матрицы в указанных формах (А и Б). Результат - в форме А.
2. Умножение - стандартный алгоритм.
3. Сравнение времени выполнения операций и объем памяти при использовании этих 2-х алг.
   при различном проценте их заполенения.
*/

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

typedef enum
{
    OP_EXIT,
    OP_LOAD_1,
    OP_LOAD_2,
    OP_FILL_RND,
    OP_MULT,
    OP_EDIT_COORD,
    OP_CMP,
    OP_COUNT,
    OP_UNKNOWN,
    OP_EOF,
} operation_t;

operation_t get_operaion(void)
{
    printf("0. Выйти\n"
           "1. Загрузить 1-ю матрицу из файла\n"
           "2. Загрузить 2-ю матрицу из файла\n"
           "3. Заполнить случайно матрицу\n"
           "4. Умножить две матрицы\n"
           "5. Редактировать матрицу\n"
           "6. Сравнить эффективность хранения\n");
    printf("Введите номер операции: ");
    int input;
    int rc = scanf("%d", &input);
    getchar();
    if (rc < 0)
        return OP_EOF;
    if (rc == 0)
        return OP_UNKNOWN;

    if (input >= OP_COUNT || input < 0)
        return OP_UNKNOWN;

    return (operation_t)input;
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

void func(void)
{
    mat_csr_t mat = {0};
    if (mat_csr_create(&mat, 1000, 1000) == ERR_OK)
    {
        mat_fill_rnd(&mat, mat.base, 40);
        FILE *fp = fopen("out.txt", "w");
        if (fp)
        {
            // mat_print(fp, &mat, mat.base);
            fclose(fp);
        }
        mat_csr_print_internal(&mat);
        mat_csr_free(&mat);
    }
}

int main()
{
    print_guide();
    // if (argc != 3)
    // {
    //     print_usage();
    //     return ERR_ARGS;
    // }

    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    bool mat1_created = false, mat2_created = false;
    mat_csr_t mat1;
    mat_csc_t mat2;
    mat_csr_t res;

err:
    while (1)
    {
        operation_t op = get_operaion();
        if (op == OP_EXIT || op == OP_EOF)
        {
            break;
        }
        else if (op == OP_LOAD_1)
        {
            if (mat1_created)
            {
                printf("Эта матрица уже введена\n");
                goto err;
            }
            printf("Введите путь до 1 матрицы: ");
            char buf[100];
            if (str_input(buf, sizeof(buf)) == ERR_OK)
            {
                fp1 = fopen(buf, "r");
                if (!fp1)
                    printf("Ошибка. Такого пути не существует\n");
                else
                {
                    size_t n, m;
                    if (fscanf(fp1, "%zu%zu", &n, &m) != 2)
                        goto err;
                    mat_csc_create(&mat1, n, m);
                    mat_csc_read(fp1, &mat1);
                    mat1_created = true;
                }
            }
        }
        else if (op == OP_LOAD_2)
        {
            if (mat2_created)
            {
                printf("Эта матрица уже введена\n");
                goto err;
            }
            printf("Введите путь до 2 матрицы: ");
            char buf[100];
            if (str_input(buf, sizeof(buf)) == ERR_OK)
            {
                fp2 = fopen(buf, "r");
                if (!fp2)
                    printf("Ошибка. Такого пути не существует\n");
                else
                {
                    size_t n, m;
                    if (fscanf(fp2, "%zu%zu", &n, &m) != 2)
                        goto err;
                    mat_csc_create(&mat2, n, m);
                    mat_csc_read(fp2, &mat2);
                    mat2_created = true;
                }
            }
        }
        else if (op == OP_MULT)
        {
            mat_csr_create(&res, mat1.base.n, mat2.base.m);
            if (mat_multiply(&mat1, mat1.base, &mat2, mat2.base, &res, res.base) != ERR_OK)
                printf("Ошибка при умножении матрицы\n");
            else
                mat_print(stdout, &res, res.base);
        }
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
