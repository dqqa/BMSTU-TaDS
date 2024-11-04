#ifndef QUEUE_LIST_H__
#define QUEUE_LIST_H__

#include "common.h"
#include <stddef.h>

typedef struct __node_t
{
    void *data;
    struct __node_t *next;
} node_t;

// Universal queue type
typedef struct __queue_list
{
    queue_base_t base;
    node_t *head;
    node_t *end;
    size_t size;
} queue_list_t;

typedef void (*queue_list_apply_fn_t)(node_t *node, void *arg);

int queue_list_create(queue_list_t *q);
void queue_list_free(queue_list_t *q);

int queue_list_peek(queue_list_t *q, void *dst, size_t data_size);

int queue_list_push(void *q, const void *src, size_t data_size);
int queue_list_pop(void *q, void *dst, size_t data_size);

bool queue_list_is_empty(const void *queue);

void queue_list_apply(queue_list_t *q, queue_list_apply_fn_t func, void *arg);

#endif // QUEUE_LIST_H__
