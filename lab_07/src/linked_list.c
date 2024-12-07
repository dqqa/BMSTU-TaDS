#include "linked_list.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Добавляет в конец списка элемент
 *
 * @param head Двойной указатель на голову
 * @param key Ключ
 * @param val Значение
 * @return int Код ошибки
 */
int push_back(avl_tree_t **head, const char *key, int val)
{
    avl_tree_t *new_node = avl_node_create();
    if (new_node == NULL)
        return ERR_ALLOC;

    new_node->key = key;
    new_node->value = val;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        avl_tree_t *tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = new_node;
    }

    return ERR_OK;
}

/**
 * @brief Освобождает память из-под списка
 *
 * @param head Двойной указатель на голову
 */
void list_free(avl_tree_t **head)
{
    avl_tree_t *tmp = *head;
    while (tmp)
    {
        avl_tree_t *next = tmp->next;
        free(tmp);
        tmp = next;
    }

    *head = NULL;
}

/**
 * @brief Применяет функцию к каждому узлу списка
 *
 * @param head Указатель на голову
 * @param func Указатель на функцию
 * @param arg Параметр для функции
 */
void list_apply(avl_tree_t *head, list_apply_fn_t func, void *arg)
{
    avl_tree_t *tmp = head;
    while (tmp != NULL)
    {
        func(tmp->key, &tmp->value, arg);
        tmp = tmp->next;
    }
}

/**
 * @brief Ищет в односвязном списке узел по ключу
 *
 * @param head Указатель на голову списка
 * @param key Ключ
 * @return const node_t* Найденный узел
 */
const avl_tree_t *list_search_by_key(const avl_tree_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    const avl_tree_t *tmp = head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
            return tmp;
        tmp = tmp->next;
    }

    return NULL;
}

/**
 * @brief Удаляет в односвязном списке узел по ключу
 *
 * @param head Указатель на голову списка
 * @param key Ключ
 * @return int Код ошибки
 */
int list_remove_by_key(avl_tree_t **head, const char *key)
{
    if (*head == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    if (strcmp((*head)->key, key) == 0)
    {
        avl_tree_t *next = (*head)->next;
        free(*head);
        *head = next;

        return ASSOC_ARRAY_OK;
    }

    avl_tree_t *tmp = *head;
    avl_tree_t *prev = *head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            avl_tree_t *next = tmp->next;
            free(tmp);
            prev->next = next;
            return ASSOC_ARRAY_OK;
        }
        prev = tmp;
        tmp = tmp->next;
    }

    return ASSOC_ARRAY_NOT_FOUND;
}

/**
 * @brief Ищет в односвязном списке узел с минимальным ключом
 * 
 * @param head Указатель на голову списка
 * @return const node_t* Указатель на узел с минимальным ключом
 */
const avl_tree_t *list_find_min(const avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    const avl_tree_t *tmp = head;
    const avl_tree_t *min = tmp;
    tmp = tmp->next;

    while (tmp)
    {
        if (strcmp(min->key, tmp->key) > 0)
            min = tmp;

        tmp = tmp->next;
    }

    return min;
}

/**
 * @brief Ищет в односвязном списке узел с максимальным ключом
 * 
 * @param head Указатель на голову списка
 * @return const node_t* Указатель на узел с максимальным ключом
 */
const avl_tree_t *list_find_max(const avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    const avl_tree_t *tmp = head;
    const avl_tree_t *max = tmp;
    tmp = tmp->next;

    while (tmp)
    {
        if (strcmp(max->key, tmp->key) < 0)
            max = tmp;

        tmp = tmp->next;
    }

    return max;
}
