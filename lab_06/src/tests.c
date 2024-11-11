#include "errors.h"
#include "file_reader.h"
#include "mymenu.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int test_tree(void)
{
    printf("Проверка двоичного дерева поиска. В каждой вершине должно располагаться одно слово.\n")
    bool need_exit = false;
    int rc = ERR_OK;
    tree_t *tree = NULL;

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

            rc = tree_insert_str(&tree, buf);
            if (rc != ERR_OK)
                goto loc_err;
        }
        else if (op == TEST_MENU_REMOVE)
        {
            char buf[256];
            printf("Введите слово: ");
            if ((rc = str_input(stdin, buf, sizeof(buf))) != ERR_OK)
                goto loc_err;

            rc = tree_remove_str(&tree, buf);
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
            tree_search_symbol(tree, symbol, &cnt);

            printf("Количество найденных элементов, начинающиеся на букву '%c': %zu\n", symbol, cnt);
            printf("Описание графа на языке DOT:\n");
            tree_to_graphviz(stdout, "tree", tree);
            tree_repeat_reset(tree);
        }
        else if (op == TEST_MENU_SHOW)
        {
            tree_to_graphviz(stdout, "tree", tree);
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
