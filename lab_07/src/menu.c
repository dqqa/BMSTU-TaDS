#include "menu.h"
#include "avl_tree.h"
#include "bst_tree.h"
#include "ht_chain.h"
#include "ht_closed.h"
#include "str.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

main_menu_action_t get_main_menu_act(void)
{
    printf("\n1. Выход\n"
           "2. Проверка работы ДДП\n"
           "3. Проверка работы АВЛ-дерева\n"
           "4. Проверка работы хэш таблицы с открытым хешированием\n"
           "5. Проверка работы хэш таблицы с закрытым хешированием\n"
           "6. Сравнение производительности\n"
           "7. Изменение параметров\n");
    printf("Введите операцию: ");

    int act;
    int rc = scanf("%d", &act);
    if (rc < 0)
        return MAIN_EXIT;

    if (rc == 0)
    {
        while (getchar() != '\n')
            ;
        return MAIN_UNKNOWN;
    }

    getchar();
    if (act < 1 || act > MAIN_COUNT)
        return MAIN_UNKNOWN;

    return (main_menu_action_t)(act - 1);
}

tree_action_t get_tree_menu_act(void)
{
    printf("\n1. Выход\n"
           "2. Загрузить слова из файла\n"
           "3. Удалить слова, начинающиеся на указанную букву\n"
           "4. Вывести дерево на экран\n"
           "5. Поиск в дереве слова\n"
           "6. Поиск в дереве слов, начинающихся на букву\n"
           "7. Добавление элемента в дерево\n");
    printf("Введите операцию: ");

    int act;
    int rc = scanf("%d", &act);
    if (rc < 0)
        return TREE_ACT_EXIT;

    if (rc == 0)
    {
        while (getchar() != '\n')
            ;
        return TREE_ACT_UNKNOWN;
    }

    getchar();
    if (act < 1 || act > TREE_ACT_COUNT)
        return TREE_ACT_UNKNOWN;

    return (tree_action_t)(act - 1);
}

ht_action_t get_ht_menu_act(void)
{
    printf("\n1. Выход\n"
           "2. Загрузить слова из файла\n"
           "3. Вывести хэш-таблицу на экран\n"
           "4. Поиск слова в хэш-таблице\n"
           "5. Удаление слова из хэш-таблицы\n"
           "6. Добавить элемент в хэш-таблицу\n");
    printf("Введите операцию: ");

    int act;
    int rc = scanf("%d", &act);
    if (rc < 0)
        return HT_ACT_EXIT;

    if (rc == 0)
    {
        while (getchar() != '\n')
            ;
        return HT_ACT_UNKNOWN;
    }

    getchar();
    if (act < 1 || act > HT_ACT_COUNT)
        return HT_ACT_UNKNOWN;

    return (ht_action_t)(act - 1);
}

int check_bst(void)
{
    bst_tree_t *tree = NULL;
    char *filepath = NULL;
    int rc = ERR_OK;

    bool should_exit = false;
    while (!should_exit)
    {
        tree_action_t act = get_tree_menu_act();
        if (act == TREE_ACT_EXIT)
        {
            should_exit = true;
        }
        else if (act == TREE_ACT_LOAD_FILE)
        {
            if (tree != NULL)
            {
                rc = ERR_REPEAT;
                goto err;
            }

            printf("Введите путь до файла: ");
            filepath = get_str(stdin, NULL);
            if (filepath == NULL)
            {
                rc = ERR_ALLOC;
                goto load_err;
            }

            rc = bst_load_file_ex(filepath, &tree);

            load_err:
            free(filepath);
            filepath = NULL;
        }
        else if (act == TREE_ACT_DELETE_WORDS)
        {
            printf("Введите символ, для удаления слов, начинающихся на него: ");
            char s;
            if (scanf("%c", &s) != 1)
            {
                rc = ERR_IO;
                goto err;
            }

            rc = bst_remove_nodes_starting_with(&tree, s);
        }
        else if (act == TREE_ACT_SHOW)
        {
            rc = bst_save_tmp_open(tree);
        }
        else if (act == TREE_ACT_SEARCH_WORD)
        {
            printf("Введите строку для поиска: ");
            char *search_term = get_str(stdin, NULL);
            if (!search_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            bst_tree_t *node = bst_search(tree, search_term);
            free(search_term);

            if (node != NULL)
                printf("Строка найдена!\n");
            else
                printf("Строка не найдена!\n");

            bst_save_tmp_open(tree);
            bst_repeat_reset(tree);
        }
        else if (act == TREE_ACT_SEARCH_STARTING)
        {
            char search_term;
            size_t cnt = 0;

            printf("Введите символ для поиска: ");
            if (scanf("%c", &search_term) != 1)
            {
                rc = ERR_IO;
                goto err;
            }

            bst_search_symbol(tree, search_term, &cnt);
            printf("Найдено %zu слов\n", cnt);

            bst_save_tmp_open(tree);
            bst_repeat_reset(tree);
        }
        else if (act == TREE_ACT_ADD)
        {
            printf("Введите строку для добавления: ");
            char *add_term = get_str(stdin, NULL);
            if (!add_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = bst_insert_str(&tree, add_term);
            free(add_term);
        }
        else
        {
            rc = ERR_PARAM;
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    free(filepath);
    bst_free(tree);
    return rc;
}

int check_avl(void)
{
    avl_tree_t *tree = NULL;
    char *filepath = NULL;
    int rc = ERR_OK;

    bool should_exit = false;
    while (!should_exit)
    {
        tree_action_t act = get_tree_menu_act();
        if (act == TREE_ACT_EXIT)
        {
            should_exit = true;
        }
        else if (act == TREE_ACT_LOAD_FILE)
        {
            if (tree != NULL)
            {
                rc = ERR_REPEAT;
                goto err;
            }

            printf("Введите путь до файла: ");
            filepath = get_str(stdin, NULL);
            if (filepath == NULL)
            {
                rc = ERR_ALLOC;
                goto load_err;
            }

            rc = avl_load_file_ex(filepath, &tree);

            load_err:
            free(filepath);
            filepath = NULL;
        }
        else if (act == TREE_ACT_DELETE_WORDS)
        {
            printf("Введите символ, для удаления слов, начинающихся на него: ");
            char s;
            if (scanf("%c", &s) != 1)
            {
                rc = ERR_IO;
                goto err;
            }

            rc = avl_remove_nodes_starting_with(&tree, s);
        }
        else if (act == TREE_ACT_SHOW)
        {
            rc = avl_save_tmp_open(tree);
        }
        else if (act == TREE_ACT_SEARCH_WORD)
        {
            printf("Введите строку для поиска: ");
            char *search_term = get_str(stdin, NULL);
            if (!search_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            avl_tree_t *node = avl_search(tree, search_term);
            free(search_term);

            if (node != NULL)
                printf("Строка найдена!\n");
            else
                printf("Строка не найдена!\n");

            avl_save_tmp_open(tree);
            avl_repeat_reset(tree);
        }
        else if (act == TREE_ACT_SEARCH_STARTING)
        {
            char search_term;
            size_t cnt = 0;

            printf("Введите символ для поиска: ");
            if (scanf("%c", &search_term) != 1)
            {
                rc = ERR_IO;
                goto err;
            }

            avl_search_symbol(tree, search_term, &cnt);
            printf("Найдено %zu слов\n", cnt);

            avl_save_tmp_open(tree);
            avl_repeat_reset(tree);
        }
        else if (act == TREE_ACT_ADD)
        {
            printf("Введите строку для добавления: ");
            char *add_term = get_str(stdin, NULL);
            if (!add_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = avl_insert_str(&tree, add_term);
            free(add_term);
        }
        else
        {
            rc = ERR_PARAM;
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    free(filepath);
    avl_free(&tree);

    return rc;
}

static void ht_open_show_callback(list_t *node, void *ind)
{
    // printf("ind: %zu, list addr: %p => %s\n", *(size_t *)ind, (void *)node, node->key);
    printf("list addr: %p => %s\n", (void *)node, node->key);
    (void)ind;
}

int check_ht_open(void)
{
    ht_chain_t *ht = NULL;
    char *filepath = NULL;
    int rc = ERR_OK;

    bool should_exit = false;
    while (!should_exit)
    {
        ht_action_t act = get_ht_menu_act();
        if (act == HT_ACT_EXIT)
        {
            should_exit = true;
        }
        else if (act == HT_ACT_LOAD_FILE)
        {
            if (ht != NULL)
            {
                rc = ERR_REPEAT;
                goto err;
            }

            printf("Введите путь до файла: ");
            filepath = get_str(stdin, NULL);
            if (filepath == NULL)
            {
                rc = ERR_ALLOC;
                goto load_err;
            }

            rc = ht_chain_load_file_ex(filepath, &ht);

            printf("Размер хэш-таблицы (количество списков): %zu\n", ht->size);

            load_err:
            free(filepath);
            filepath = NULL;
        }
        else if (act == HT_ACT_REMOVE)
        {
            printf("Введите строку для удаления: ");
            char *remove_term = get_str(stdin, NULL);
            if (remove_term == NULL)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = ht_chain_remove(ht, remove_term);
            free(remove_term);

            if (rc == ERR_OK)
                printf("Успешно удалено!\n");
        }
        else if (act == HT_ACT_SEARCH)
        {
            printf("Введите строку для поиска: ");
            char *search_term = get_str(stdin, NULL);
            if (search_term == NULL)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = ht_chain_find(ht, search_term);
            free(search_term);
            if (rc == ERR_OK)
                printf("Значение найдено!\n");
        }
        else if (act == HT_ACT_SHOW)
        {
            ht_chain_each(ht, ht_open_show_callback, NULL);
        }
        else if (act == HT_ACT_ADD)
        {
            printf("Введите строку для добавления: ");
            char *add_term = get_str(stdin, NULL);
            if (!add_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            bool is_restruct;
            rc = ht_chain_insert(&ht, add_term, &is_restruct);
            free(add_term);

            printf(is_restruct ? "Понадобилась реструктуризация\n" : "Реструктуризаця не понадобилась\n");
        }
        else
        {
            rc = ERR_PARAM;
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    ht_chain_free(&ht);
    return rc;
}

void ht_closed_show_callback(struct __ht_closed_item *item, void *arg)
{
    (void)arg;
    printf("%p => %s\n", (void *)item, item->data);
}

int check_ht_closed(void)
{
    ht_closed_t *ht = NULL;
    char *filepath = NULL;
    int rc = ERR_OK;

    bool should_exit = false;
    while (!should_exit)
    {
        ht_action_t act = get_ht_menu_act();
        if (act == HT_ACT_EXIT)
        {
            should_exit = true;
        }
        else if (act == HT_ACT_LOAD_FILE)
        {
            if (ht != NULL)
            {
                rc = ERR_REPEAT;
                goto err;
            }

            printf("Введите путь до файла: ");
            filepath = get_str(stdin, NULL);
            if (filepath == NULL)
            {
                rc = ERR_ALLOC;
                goto load_err;
            }

            rc = ht_closed_load_file_ex(filepath, &ht);

            printf("Размер хэш-таблицы: %zu\n", ht->size);

            load_err:
            free(filepath);
            filepath = NULL;
        }
        else if (act == HT_ACT_REMOVE)
        {
            printf("Введите строку для удаления: ");
            char *remove_term = get_str(stdin, NULL);
            if (remove_term == NULL)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = ht_closed_remove(ht, remove_term);
            free(remove_term);

            if (rc == ERR_OK)
                printf("Успешно удалено!\n");
        }
        else if (act == HT_ACT_SEARCH)
        {
            printf("Введите строку для поиска: ");
            char *search_term = get_str(stdin, NULL);
            if (search_term == NULL)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            rc = ht_closed_find(ht, search_term);
            free(search_term);
            if (rc == ERR_OK)
                printf("Значение найдено!\n");
        }
        else if (act == HT_ACT_SHOW)
        {
            ht_closed_each(ht, ht_closed_show_callback, NULL);
        }
        else if (act == HT_ACT_ADD)
        {
            printf("Введите строку для добавления: ");
            char *add_term = get_str(stdin, NULL);
            if (!add_term)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            bool is_restruct;
            rc = ht_closed_insert(&ht, add_term, &is_restruct);
            free(add_term);

            printf(is_restruct ? "Понадобилась реструктуризация" : "Реструктуризаця не понадобилась");
        }
        else
        {
            rc = ERR_PARAM;
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    ht_closed_free(&ht);
    return rc;
}

int perf_cmp(void);
