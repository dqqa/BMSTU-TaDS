#include "errors.h"
#include "file_reader.h"
#include "mymenu.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int str_input(FILE *fp, char *buf, size_t buf_size)
{
    if (!fgets(buf, buf_size, fp))
        return ERR_IO;

    char *newline = strchr(buf, '\n');
    if (newline)
        *newline = 0;
    else
        return ERR_OVERFLOW;

    return ERR_OK;
}

static void tree_printer(tree_t *t, void *fmt)
{
    printf(fmt, t->data);
}

int test_tree(void)
{
    printf("Проверка двоичного дерева поиска. В каждой вершине должно располагаться одно слово.\n"
           "Каждое слово должно состояить только из латинских символов или цифр\n");
    bool need_exit = false;
    int rc = ERR_OK;
    tree_t *tree = NULL;
    struct timespec start, end;

    while (!need_exit)
    {
        test_menu_op_t op = get_test_menu_op();
        if (op == TEST_MENU_EXIT || op == TEST_MENU_EOF)
        {
            need_exit = true;
        }
        else if (op == TEST_MENU_ADD)
        {
            char buf[256];
            printf("Введите слово: ");
            if ((rc = str_input(stdin, buf, sizeof(buf))) != ERR_OK)
                goto loc_err;

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = tree_insert_str(&tree, buf);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            float t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            printf("Время, затраченное на добавление: %.2f мкс.\n", t);
            if (rc != ERR_OK)
                goto loc_err;
        }
        else if (op == TEST_MENU_REMOVE)
        {
            char buf[256];
            printf("Введите слово: ");
            if ((rc = str_input(stdin, buf, sizeof(buf))) != ERR_OK)
                goto loc_err;

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = tree_remove_str(&tree, buf);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            float t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            printf("Время, затраченное на удаление: %.2f мкс.\n", t);
        }
        else if (op == TEST_MENU_SEARCH)
        {
            char symbol;
            printf("Введите символ для поиска: ");
            if (scanf("%c", &symbol) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            size_t cnt = 0;

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            tree_search_symbol(tree, symbol, &cnt);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            float t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            printf("Время, затраченное на поиск: %.2f мкс.\n", t);

            printf("Количество найденных элементов, начинающиеся на букву '%c': %zu\n", symbol, cnt);
            // printf("Описание графа на языке DOT:\n");
            // tree_to_graphviz(stdout, "tree", tree);
            rc = tree_save_tmp_open(tree);
            tree_repeat_reset(tree);
        }
        else if (op == TEST_MENU_SHOW)
        {
            rc = tree_save_tmp_open(tree);
            // tree_to_graphviz(stdout, "tree", tree);
        }
        else if (op == TEST_MENU_SHOW_PRE)
        {
            tree_apply_pre(tree, tree_printer, "%s ");
        }
        else if (op == TEST_MENU_SHOW_IN)
        {
            tree_apply_in(tree, tree_printer, "%s ");
        }
        else if (op == TEST_MENU_SHOW_POST)
        {
            tree_apply_post(tree, tree_printer, "%s ");
        }
        else
        {
            printf("Ошибка!\n");
        }

        loc_err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    err:
    tree_free(tree);

    return rc;
}
