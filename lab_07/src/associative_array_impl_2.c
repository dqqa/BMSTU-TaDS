#include "associative_array.h"
#include "hasher.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

// Размер - простое число для уменьшения количества коллизий
#define HT_SIZE 997

// Хэш таблица (метод цепочек - массив списков)
struct assoc_array_type
{
    avl_tree_t *table[HT_SIZE];
};

assoc_array_t assoc_array_create(void)
{
    assoc_array_t arr = calloc(1, sizeof(struct assoc_array_type));

    return arr;
}

void assoc_array_destroy(assoc_array_t *arr)
{
    if (arr == NULL || *arr == NULL)
        return;

    for (size_t i = 0; i < HT_SIZE; i++)
        list_free((*arr)->table + i);

    free(*arr);

    *arr = NULL;
}

assoc_array_error_t assoc_array_insert(assoc_array_t arr, const char *key, int num)
{
    if (arr == NULL || key == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    if (strlen(key) == 0)
        return ASSOC_ARRAY_INVALID_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % HT_SIZE;

    if (list_search_by_key(arr->table[index], key) != NULL)
        return ASSOC_ARRAY_KEY_EXISTS;

    return push_back(&arr->table[index], key, num);
}

assoc_array_error_t assoc_array_find(const assoc_array_t arr, const char *key, int **num)
{
    if (arr == NULL || key == NULL || num == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    if (strlen(key) == 0)
        return ASSOC_ARRAY_INVALID_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % HT_SIZE;

    avl_tree_t *node = (avl_tree_t *)list_search_by_key(arr->table[index], key);
    if (node == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &node->value;
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_remove(assoc_array_t arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    if (strlen(key) == 0)
        return ASSOC_ARRAY_INVALID_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % HT_SIZE;

    return list_remove_by_key(&arr->table[index], key);
}

assoc_array_error_t assoc_array_clear(assoc_array_t arr)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    for (size_t i = 0; i < HT_SIZE; i++)
        list_free(&arr->table[i]);

    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_each(const assoc_array_t arr, void (*action)(const char *key, int *num, void *param), void *param)
{
    if (arr == NULL || action == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    for (size_t i = 0; i < HT_SIZE; i++)
        list_apply(arr->table[i], action, param);

    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_min(const assoc_array_t arr, int **num)
{
    if (arr == NULL || num == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    avl_tree_t *min = NULL;

    for (size_t i = 0; i < HT_SIZE; i++)
    {
        if (min == NULL)
        {
            min = (avl_tree_t *)list_find_min(arr->table[i]);
        }
        else
        {
            avl_tree_t *new_min = (avl_tree_t *)list_find_min(arr->table[i]);
            if (new_min != NULL && strcmp(min->key, new_min->key) > 0)
                min = new_min;
        }
    }

    if (min == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &min->value;
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_max(const assoc_array_t arr, int **num)
{
    if (arr == NULL || num == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    avl_tree_t *max = NULL;

    for (size_t i = 0; i < HT_SIZE; i++)
    {
        if (max == NULL)
        {
            max = (avl_tree_t *)list_find_max(arr->table[i]);
        }
        else
        {
            avl_tree_t *new_max = (avl_tree_t *)list_find_max(arr->table[i]);
            if (new_max != NULL && strcmp(max->key, new_max->key) < 0)
                max = new_max;
        }
    }

    if (max == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &max->value;
    return ASSOC_ARRAY_OK;
}
