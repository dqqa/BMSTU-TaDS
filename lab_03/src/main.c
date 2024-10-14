#include "errors.h"
#include "sparse_mat_a.h"
#include "sparse_mat_b.h"
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
            mat_a_t mat_a = {0}, mat_b = {0};
            mat_a_t res = {0};
            size_t n1, m1, n2, m2;
            if (fscanf(fp1, "%zu%zu", &n1, &m1) == 2)
            {
                mat_a_create(&mat_a, n1, m1);
                mat_a_read(fp1, &mat_a);

                printf("Matrix 1:\n");
                mat_a_save(stdout, &mat_a);
            }
            if (fscanf(fp2, "%zu%zu", &n2, &m2) == 2)
            {
                mat_a_create(&mat_b, n2, m2);
                mat_a_read(fp2, &mat_b);

                printf("\nMatrix 2:\n");
                mat_a_save(stdout, &mat_b);
            }

            mat_a_create(&res, n1, m2);
            mat_multiply(&mat_a, mat_a.base, &mat_b, mat_b.base, &res, res.base);

            printf("Result:\n");
            mat_a_save(stdout, &res);
            
            mat_a_free(&mat_a);
            mat_a_free(&mat_b);
            mat_a_free(&res);
            
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
