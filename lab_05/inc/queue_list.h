#ifndef QUEUE_LIST_H__
#define QUEUE_LIST_H__

#include "common.h"
#include <stddef.h>

typedef struct __node_t
{
    void *data;
    struct __node_t *next;
} node_t;

typedef struct __queue_list
{
    node_t *head;
    node_t *end;
    size_t size;
} queue_list_t;

typedef void (*queue_list_apply_fn_t)(node_t *node, void *arg);

int queue_list_create(queue_list_t *q);
void queue_list_free(queue_list_t *q);

int queue_list_peek(queue_list_t *q, void *dst, size_t data_size);

int queue_list_push(queue_list_t *q, const data_t *src, size_t data_size);
int queue_list_pop(queue_list_t *q, data_t *dst, size_t data_size);

int queue_list_apply(queue_list_t *q, queue_list_apply_fn_t func);

#endif // QUEUE_LIST_H__
