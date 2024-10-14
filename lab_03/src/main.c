#include "errors.h"
#include "mat_csr.h"
#include "mat_csc.h"
#include <stdio.h>

/*
TODO:

1. Умножить две матрицы в указанных формах (А и Б). Результат - в форме А.
2. Умножение - стандартный алгоритм.
3. Сравнение времени выполнения операций и объем памяти при использовании этих 2-х алг.
   при различном проценте их заполенения.
*/

void print_usage(void)
{
    fprintf(stderr, "Usage: app.exe <matrix 1> <matrix 2>\n");
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        print_usage();
        return ERR_ARGS;
    }

    FILE *fp1 = fopen(argv[1], "r");
    if (fp1)
    {
        FILE *fp2 = fopen(argv[2], "r");
        if (fp2)
        {
            mat_csr_t mat_a = {0}, mat_b = {0};
            mat_csr_t res = {0};
            size_t n1, m1, n2, m2;
            if (fscanf(fp1, "%zu%zu", &n1, &m1) == 2)
            {
                mat_csr_create(&mat_a, n1, m1);
                mat_csr_read(fp1, &mat_a);

                printf("Matrix 1:\n");
                mat_print(stdout, &mat_a, mat_a.base);
            }
            if (fscanf(fp2, "%zu%zu", &n2, &m2) == 2)
            {
                mat_csr_create(&mat_b, n2, m2);
                mat_csr_read(fp2, &mat_b);

                printf("\nMatrix 2:\n");
                mat_print(stdout, &mat_b, mat_b.base);
            }

            mat_csr_create(&res, n1, m2);
            mat_multiply(&mat_a, mat_a.base, &mat_b, mat_b.base, &res, res.base);

            printf("\nResult:\n");
            mat_print(stdout, &res, res.base);

            mat_csr_free(&mat_a);
            mat_csr_free(&mat_b);
            mat_csr_free(&res);

            fclose(fp2);
        }
        fclose(fp1);
    }
    else
    {
        print_err(ERR_IO);
        return ERR_IO;
    }

    return 0;
}
