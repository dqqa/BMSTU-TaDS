#include "queue_list.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>

int queue_list_create(queue_list_t *q)
{
    q->head = NULL;
    q->end = NULL;
    q->size = 0;

    return ERR_OK;
}

void queue_list_free(queue_list_t *q)
{
    node_t *cur = q->head;
    while (cur)
    {
        node_t *tmp = cur->next;

        free(cur->data);
        free(cur);

        cur = tmp;
    }
}

int queue_list_peek(queue_list_t *q, void *dst, size_t data_size)
{
    if (!q->size)
        return ERR_UNDERFLOW;

    if (dst)
        memcpy(dst, q->head->data, data_size);

    return ERR_OK;
}

int queue_list_push(queue_list_t *q, const void *src, size_t data_size)
{
    int rc = ERR_OK;

    void *new_data = malloc(data_size);
    node_t *new_node = malloc(sizeof(*new_node));
    if (!new_data || !new_node)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    memcpy(new_data, src, data_size);
    new_node->next = NULL;
    new_node->data = new_data;

    if (q->end)
        q->end->next = new_node;
    if (!q->head)
        q->head = new_node;

    q->end = new_node;
    q->size++;

    return ERR_OK;

    err:
    free(new_data);
    free(new_node);
    return rc;
}

int queue_list_pop(queue_list_t *q, void *dst, size_t data_size)
{
    if (!q->size)
        return ERR_UNDERFLOW;

    if (dst)
        memcpy(dst, q->head->data, data_size);

    node_t *next = q->head->next;

    free(q->head->data);
    free(q->head);

    q->head = next;
    q->size--;
    
    return ERR_OK;
}

void queue_list_apply(queue_list_t *q, queue_list_apply_fn_t func, void *arg)
{
    node_t *node = q->head;
    while (node)
    {
        func(node, arg);
        node = node->next;
    }
}
