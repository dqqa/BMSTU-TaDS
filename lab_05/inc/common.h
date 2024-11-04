#ifndef COMMON_H__
#define COMMON_H__

#include <inttypes.h>
#include <stdbool.h>

typedef int32_t data_t;
typedef int (*queue_push_fn_t)(void *q, const data_t *src);
typedef int (*queue_pop_fn_t)(void *q, data_t *dst);
typedef bool (*queue_is_empty_fn_t)(const void *q);

typedef struct __queue_base
{
    queue_push_fn_t push;
    queue_pop_fn_t pop;
    queue_is_empty_fn_t is_empty;
} queue_base_t;

#endif // COMMON_H__
