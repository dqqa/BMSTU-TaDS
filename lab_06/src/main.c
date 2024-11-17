#include "errors.h"
#include "file_reader.h"
#include "mymenu.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define return_defer(num) \
    do                    \
    {                     \
        result = (num);   \
        goto defer;       \
    } while (0)

/* FORWARD DECLARATIONS */

int perf_cmp(void);
int test_tree(void);

void tree_printer(tree_t *t, void *fmt)
{
    printf(fmt, t->data);
}

int main2(void)
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

void print_guide(void)
{
    printf("Программа для построения двоичного дерева поиска из слов в файле.\n"
           "Предупреждение: в текстовом файле должно быть одно слово на отдельной строке.\n");
}

int main(void)
{
    print_guide();
    bool need_exit = false;
    int rc = ERR_OK;
    tree_t *tree = NULL;

    while (!need_exit)
    {
        main_menu_op_t op = get_main_menu_op();
        if (op == MAIN_MENU_EXIT || op == MAIN_MENU_EOF)
        {
            need_exit = true;
        }
        else if (op == MAIN_MENU_TEST_TREE)
        {
            rc = test_tree();
        }
        else if (op == MAIN_MENU_LOAD)
        {
            if (tree)
            {
                rc = ERR_REPEAT;
                goto err;
            }

            printf("Введите путь до файла: ");
            char buf[256];
            if (!fgets(buf, sizeof(buf), stdin))
            {
                rc = ERR_IO;
                goto err;
            }

            char *newline = strchr(buf, '\n');
            if (newline)
            {
                *newline = 0;
            }
            else
            {
                rc = ERR_IO;
                goto err;
            }

            rc = file_load_ex(buf, &tree);
        }
        else if (op == MAIN_MENU_SEARCH)
        {
            char symbol;
            if (scanf("%c", &symbol) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            size_t cnt = 0;
            tree_search_symbol(tree, symbol, &cnt);

            printf("Количество найденных элементов, начинающиеся на букву '%c': %zu\n", symbol, cnt);
            // printf("Описание графа на языке DOT:\n");
            // tree_to_graphviz(stdout, "tree", tree);
            rc = tree_save_tmp_open(tree);
            tree_repeat_reset(tree);
        }
        else if (op == MAIN_MENU_PERFORMANCE_CMP)
        {
            rc = perf_cmp();
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    tree_free(tree);
    return rc;
}
