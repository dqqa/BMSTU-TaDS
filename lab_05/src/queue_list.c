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
    if (!q->end)
        return ERR_UNDERFLOW;

    if (dst)
        memcpy(dst, q->end->data, data_size);

    return ERR_OK;
}

int queue_list_push(queue_list_t *q, const data_t *src, size_t data_size)
{
    int rc = ERR_OK;

    void *new_data = malloc(data_size);
    node_t *new_node = malloc(sizeof(*new_node));
    if (!new_data || !new_node)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    memcpy(new_node->data, src, data_size);
    new_node->next = NULL;

    q->end->next = new_node;
    q->end = new_node;
    q->size++;

    return ERR_OK;

    err:
    free(new_data);
    free(new_node);
    return rc;
}

int queue_list_pop(queue_list_t *q, data_t *dst, size_t data_size)
{
    // TODO
}

int queue_list_apply(queue_list_t *q, queue_list_apply_fn_t func);
