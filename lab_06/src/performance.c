#include "errors.h"
#include "file_reader.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define ITER_COUNT 1000
#define TEST_FILE "tests/perf.txt"

int perf_cmp(void)
{
    char symbol;
    printf("Введите символ для поиска: ");
    if (scanf("%c", &symbol) != 1)
        return ERR_IO;

    struct timespec start, end;
    double elapsed_time = 0;
    tree_t *tree = NULL;
    int rc = file_load_ex(TEST_FILE, &tree);
    assert(rc == ERR_OK || rc == ERR_REPEAT);

    for (size_t i = 0; i < ITER_COUNT; i++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        tree_search_symbol(tree, symbol, NULL);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        elapsed_time += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

        tree_repeat_reset(tree);
    }
    elapsed_time /= ITER_COUNT;
    tree_free(tree);
    printf("Поиск слов при помощи бинарного дерева поиска: %.2f мкс.\n", elapsed_time);

    elapsed_time = 0;
    FILE *fp = fopen(TEST_FILE, "r");
    assert(fp);

    for (size_t i = 0; i < ITER_COUNT; i++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        file_search_symbol(fp, symbol, NULL);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        elapsed_time += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

        rewind(fp);
    }

    elapsed_time /= ITER_COUNT;
    printf("Обычный поиск слов в файле: %.2f мкс.\n", elapsed_time);
    fclose(fp);

    return ERR_OK;
}
