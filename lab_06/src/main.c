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

void print_guide(void)
{
    printf("Программа для построения двоичного дерева поиска из слов в файле.\n"
           "Предупреждение: в текстовом файле должно быть одно слово на каждой строке.\n");
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
            printf("Введите символ для поиска: ");
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
        else
        {
            printf("Ошибка!\n");
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
