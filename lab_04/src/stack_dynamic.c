#include "stack_dynamic.h"
#include "common.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>

int stack_dyn_create(stack_dyn_t *s)
{
    s->head = NULL;
    s->size = 0;

    return ERR_OK;
}

int stack_dyn_push(stack_dyn_t *s, const void *src, size_t src_size)
{
    int rc = ERR_OK;
    node_t *new_node = malloc(sizeof(*new_node));
    void *new_data = malloc(src_size);
    if (!new_node || !new_data)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    new_node->data = new_data;
    new_node->next = s->head;

    memcpy(new_node->data, src, src_size);

    s->size++;
    s->head = new_node;

    return ERR_OK;

    err:
    free(new_data);
    free(new_node);

    return rc;
}

int stack_dyn_pop(stack_dyn_t *s, void *dst, size_t dst_size)
{
    if (s->size == 0)
        return ERR_STACK_UNDERFLOW;

    node_t *node = s->head;

    if (dst)
        memcpy(dst, s->head->data, dst_size);

    s->head = s->head->next;
    s->size--;

    free(node->data);
    free(node);
    return ERR_OK;
}

void stack_dyn_free(stack_dyn_t *s)
{
    while (s->head)
    {
        node_t *node = s->head;
        s->head = s->head->next;

        free(node->data);
        free(node);
    }
}

void stack_dyn_apply(stack_dyn_t *s, stack_apply_fn func, void *arg)
{
    node_t *node = s->head;
    while (node)
    {
        func(node, arg);
        node = node->next;
    }
}

int stack_dyn_peek(stack_dyn_t *s, void *dst, size_t dst_size)
{
    if (!s->head)
        return ERR_STACK_UNDERFLOW;

    if (dst)
        memcpy(dst, s->head->data, dst_size);

    return ERR_OK;
}
