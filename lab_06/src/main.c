#define _GNU_SOURCE

#include "errors.h"
#include "file_reader.h"
#include "tree.h"
#include <stdio.h>
#include <time.h>

#define return_defer(num) \
    do                    \
    {                     \
        result = (num);   \
        goto defer;       \
    } while (0)

void tree_printer(tree_t *t, void *fmt)
{
    printf(fmt, t->data);
}

int main(void)
{
    tree_t *t = NULL;
    int rc = ERR_OK, result = ERR_OK;
    FILE *fp = NULL;
    size_t cnt = 0;
    struct timespec start, end;

    rc = file_load_ex("tests/1.txt", &t);
    if (rc != ERR_OK)
        return_defer(rc);

    // tree_to_graphviz(stdout, "tree", t);

    printf("tree_search_symbol:\n");

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    tree_search_symbol(t, '0', &cnt);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
    printf("COUNT: %zu; time: %.2f us\n\n", cnt, time);

    cnt = 0;
    printf("file_search_symbol_ex:\n");
    fp = fopen("tests/1.txt", "r");
    if (!fp)
        return_defer(ERR_IO);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    file_search_symbol(fp, '0', &cnt);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    time = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
    printf("COUNT: %zu; time: %.2f us\n\n", cnt, time);

    defer:
    tree_free(t);

    if (fp)
        fclose(fp);

    return result;
}
