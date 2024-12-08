#include "linked_list.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Создает узел односвязного списка
 * 
 * @return list_t* Указатель на узел
 */
list_t *list_node_create(void)
{
    list_t *new_node = malloc(sizeof(*new_node));
    if (new_node)
    {
        new_node->key = NULL;
        new_node->next = NULL;
    }

    return new_node;
}

/**
 * @brief Добавляет в конец списка элемент
 *
 * @param head Двойной указатель на голову
 * @param key Ключ
 * @param val Значение
 * @return int Код ошибки
 */
int list_push_back(list_t **head __attribute_maybe_unused__, const char *key __attribute_maybe_unused__)
{
    // list_t *new_node = list_node_create();
    // if (new_node == NULL)
    //     return ERR_ALLOC;

    // new_node->key = key;

    // if (*head == NULL)
    // {
    //     *head = new_node;
    // }
    // else
    // {
    //     list_t *tmp = *head;
    //     while (tmp->next != NULL)
    //         tmp = tmp->next;

    //     tmp->next = new_node;
    // }

    return ERR_OK;
}

/**
 * @brief Освобождает память из-под списка
 *
 * @param head Двойной указатель на голову
 */
void list_free(list_t **head)
{
    list_t *tmp = *head;
    while (tmp)
    {
        list_t *next = tmp->next;
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
void list_apply(list_t *head, list_apply_fn_t func, void *arg)
{
    list_t *tmp = head;
    while (tmp != NULL)
    {
        func(tmp, arg);
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
const list_t *list_search_by_key(const list_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    const list_t *tmp = head;
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
int list_remove_by_key(list_t **head, const char *key)
{
    if (*head == NULL)
        return ERR_PARAM;

    if (strcmp((*head)->key, key) == 0)
    {
        list_t *next = (*head)->next;
        free(*head);
        *head = next;

        return ERR_OK;
    }

    list_t *tmp = *head;
    list_t *prev = *head;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            list_t *next = tmp->next;
            free(tmp);
            prev->next = next;
            return ERR_OK;
        }
        prev = tmp;
        tmp = tmp->next;
    }

    return ERR_NOT_FOUND;
}

/**
 * @brief Ищет в односвязном списке узел с минимальным ключом
 * 
 * @param head Указатель на голову списка
 * @return const node_t* Указатель на узел с минимальным ключом
 */
const list_t *list_find_min(const list_t *head)
{
    if (head == NULL)
        return NULL;

    const list_t *tmp = head;
    const list_t *min = tmp;
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
const list_t *list_find_max(const list_t *head)
{
    if (head == NULL)
        return NULL;

    const list_t *tmp = head;
    const list_t *max = tmp;
    tmp = tmp->next;

    while (tmp)
    {
        if (strcmp(max->key, tmp->key) < 0)
            max = tmp;

        tmp = tmp->next;
    }

    return max;
}
