#define _GNU_SOURCE

#include "avl_tree.h"
#include "menu.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t g_max_collisions = 4;

void print_guide(void)
{
    printf("Задание: Построить дерево поиска из слов текстового файла (задача №6), сбалансировать\n"
           "полученное дерево. Вывести его на экран в виде дерева. Удалить все слова,\n"
           "начинающиеся на указанную букву, в исходном и сбалансированном дереве.\n"
           "Построить хеш-таблицу из слов текстового файла. Вывести построенную таблицу\n"
           "слов на экран. Осуществить поиск и удаление введенного слова. Выполнить\n"
           "программу для различных размерностей таблицы и сравнить время удаления,\n"
           "объем памяти и количество сравнений при использовании сбалансированных\n"
           "деревьев и хеш-таблиц.\n\n");
}

int main(void)
{
    print_guide();

    bool should_exit = false;
    while (!should_exit)
    {
        main_menu_action_t act = get_main_menu_act();
        int rc = ERR_OK;
        if (act == MAIN_EXIT)
        {
            should_exit = true;
        }
        else if (act == MAIN_CHECK_BST)
        {
            rc = check_bst();
        }
        else if (act == MAIN_CHECK_AVL)
        {
            rc = check_avl();
        }
        else if (act == MAIN_CHECK_HASH_OPEN)
        {
            rc = check_ht_open();
        }
        else if (act == MAIN_CHECK_HASH_CLOSED)
        {
            rc = check_ht_closed();
        }
        else if (act == MAIN_PERF_CMP)
        {
            // rc = perf_cmp();
        }
        else if (act == MAIN_EDIT_PARAM)
        {
            printf("Введите максимальное количество коллизий для реструктуризации: ");
            size_t new_cnt;
            if (scanf("%zu", &new_cnt) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            g_max_collisions = new_cnt;
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

    return ERR_OK;
}
