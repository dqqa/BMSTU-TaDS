#include "ht_chain.h"
#include "hasher.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

ht_chain_t *ht_chain_create(void)
{
    ht_chain_t *arr = calloc(1, sizeof(*arr));

    return arr;
}

void ht_chain_destroy(ht_chain_t **arr)
{
    if (arr == NULL || *arr == NULL)
        return;

    for (size_t i = 0; i < (*arr)->size; i++)
        list_free((*arr)->table + i);

    free(*arr);

    *arr = NULL;
}

int ht_chain_insert(ht_chain_t *arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;

    if (list_search_by_key(arr->table[index], key) != NULL)
        return ERR_REPEAT;

    return list_push_back(&arr->table[index], key);
}

int ht_chain_find(const ht_chain_t *arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;

    list_t *node = (list_t *)list_search_by_key(arr->table[index], key);
    if (node == NULL)
        return ERR_NOT_FOUND;

    // *num = &node->value;
    return ERR_OK;
}

int ht_chain_remove(ht_chain_t *arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;

    return list_remove_by_key(&arr->table[index], key);
}

int ht_chain_clear(ht_chain_t *arr)
{
    if (arr == NULL)
        return ERR_PARAM;

    for (size_t i = 0; i < arr->size; i++)
        list_free(&arr->table[i]);

    return ERR_OK;
}

int ht_chain_each(const ht_chain_t *arr, list_apply_fn_t func, void *param)
{
    if (arr == NULL || func == NULL)
        return ERR_PARAM;

    for (size_t i = 0; i < arr->size; i++)
        list_apply(arr->table[i], func, param);

    return ERR_OK;
}
