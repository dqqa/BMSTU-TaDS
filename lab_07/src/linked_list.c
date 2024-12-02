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
 * @return assoc_array_error_t Код ошибки
 */
int push_back(node_t **head, const char *key, int val)
{
    node_t *new_node = node_create();
    if (new_node == NULL)
        return ERR_ALLOC;

    new_node->value = val;

    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        node_t *tmp = *head;
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
void list_free(node_t **head)
{
    node_t *tmp = *head;
    while (tmp)
    {
        node_t *next = tmp->next;
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
void list_apply(node_t *head, list_apply_fn_t func, void *arg)
{
    node_t *tmp = head;
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
const node_t *list_search_by_key(const node_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    const node_t *tmp = head;
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
 * @return assoc_array_error_t Код ошибки
 */
assoc_array_error_t list_remove_by_key(node_t **head, const char *key)
{
    if (*head == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    if (strcmp((*head)->key, key) == 0)
    {
        node_t *next = (*head)->next;
        free(*head);
        *head = next;

        return ASSOC_ARRAY_OK;
    }

    node_t *tmp = *head;
    node_t *prev = *head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            node_t *next = tmp->next;
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
const node_t *list_find_min(const node_t *head)
{
    if (head == NULL)
        return NULL;

    const node_t *tmp = head;
    const node_t *min = tmp;
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
const node_t *list_find_max(const node_t *head)
{
    if (head == NULL)
        return NULL;

    const node_t *tmp = head;
    const node_t *max = tmp;
    tmp = tmp->next;

    while (tmp)
    {
        if (strcmp(max->key, tmp->key) < 0)
            max = tmp;

        tmp = tmp->next;
    }

    return max;
}
