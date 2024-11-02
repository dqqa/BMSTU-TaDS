#include "queue_arr.h"
#include "errors.h"
#include <string.h>

int queue_arr_create(queue_arr_t *q)
{
    memset(q->data, 0, sizeof(q->data));
    q->size = 0;
    
    return ERR_OK;
}

int queue_arr_push(queue_arr_t *q, const data_t *src)
{
    if (q->size >= MAX_QUEUE_SIZE)
        return ERR_OVERFLOW;

    q->data[q->size] = *src;
    q->size++;

    return ERR_OK;
}

int queue_arr_pop(queue_arr_t *q, data_t *dst)
{
    if (q->size == 0)
        return ERR_UNDERFLOW;

    *dst = q->data[0];
    q->size--;

    memmove(q->data, q->data + 1, sizeof(q->data[0]) * (q->size));

    return ERR_OK;
}

void queue_arr_apply(queue_arr_t *q, queue_arr_apply_fn_t func, void *arg)
{
    for (size_t i = 0; i < q->size; i++)
        func(q->data + i, arg);
}
