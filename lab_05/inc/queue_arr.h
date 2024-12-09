#ifndef QUEUE_ARR_H__
#define QUEUE_ARR_H__

#include "common.h"
#include <stddef.h>

#define MAX_QUEUE_SIZE 10000

typedef struct __queue_arr_t
{
    queue_base_t base;
    data_t data[MAX_QUEUE_SIZE];
    size_t size;
} queue_arr_t;

typedef void (*queue_arr_apply_fn_t)(data_t *data, void *arg);

int queue_arr_create(queue_arr_t *q);

int queue_arr_push(void *q, const data_t *src);
int queue_arr_pop(void *q, data_t *dst);
int queue_arr_peek(void *queue, data_t *dst);

bool queue_arr_is_empty(const void *q);

void queue_arr_apply(queue_arr_t *q, queue_arr_apply_fn_t func, void *arg);

#endif // QUEUE_ARR_H__
