#include "associative_array.h"
#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>

struct assoc_array_type
{
    node_t *head;
};

assoc_array_t assoc_array_create(void)
{
    assoc_array_t assoc_arr = malloc(sizeof(struct assoc_array_type));
    if (assoc_arr != NULL)
        assoc_arr->head = NULL;

    return assoc_arr;
}

void assoc_array_destroy(assoc_array_t *arr)
{
    if (*arr == NULL)
        return;

    avl_free(&(*arr)->head);
    free(*arr);
    *arr = NULL;
}

assoc_array_error_t assoc_array_insert(assoc_array_t arr, const char *key, int num)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    node_t *newnode = node_create();
    if (newnode == NULL)
        return ASSOC_ARRAY_MEM;

    newnode->key = key;
    newnode->value = num;

    assoc_array_error_t err = avl_insert(&arr->head, newnode);
    if (err != ASSOC_ARRAY_OK)
        free(newnode);

    return err;
}

assoc_array_error_t assoc_array_find(const assoc_array_t arr, const char *key, int **num)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    node_t *node = (node_t *)avl_search(arr->head, key);
    if (node == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &node->value;
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_remove(assoc_array_t arr, const char *key)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    node_t *new_head = avl_remove(arr->head, key);
    if (new_head == NULL)
        return ASSOC_ARRAY_MEM;

    arr->head = new_head;

    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_clear(assoc_array_t arr)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    avl_free(&arr->head);
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_each(const assoc_array_t arr, void (*action)(const char *key, int *num, void *param), void *param)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    avl_apply(arr->head, action, param);
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_min(const assoc_array_t arr, int **num)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    node_t *node = (node_t *)avl_find_min(arr->head);
    if (node == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &node->value;
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t assoc_array_max(const assoc_array_t arr, int **num)
{
    if (arr == NULL)
        return ASSOC_ARRAY_INVALID_PARAM;

    node_t *node = (node_t *)avl_find_max(arr->head);
    if (node == NULL)
        return ASSOC_ARRAY_NOT_FOUND;

    *num = &node->value;
    return ASSOC_ARRAY_OK;
}
