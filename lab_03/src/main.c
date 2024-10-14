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
    fprintf(stderr, "Usage: app.exe <matrix>\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_usage();
        return ERR_ARGS;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp)
    {
        mat_a_t mat_a = {0};
        size_t n, m;
        if (fscanf(fp, "%zu%zu", &n, &m) == 2)
        {
            mat_a_create(&mat_a, n, m);
            mat_a_read(fp, &mat_a);
            
            mat_a_save(stdout, &mat_a);

            mat_a_free(&mat_a);
        }
        fclose(fp);
    }
    else
    {
        print_err(ERR_IO);
        return ERR_IO;
    }

    return 0;
}
