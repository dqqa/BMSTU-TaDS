#ifndef COMMON_H__
#define COMMON_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct __request_t
{
    float arrival_time; // Время прибытия заявки
    float service_time; // Время обслуживания
} request_t;

typedef union __data_t
{
    request_t req;
    float test_num; // ONLY FOR TESTS
} data_t;

typedef int (*queue_push_fn_t)(void *q, const data_t *src);
typedef int (*queue_pop_fn_t)(void *q, data_t *dst);
typedef int (*queue_peek_fn_t)(void *q, data_t *dst);
typedef bool (*queue_is_empty_fn_t)(const void *q);

typedef struct __queue_base
{
    /* VIRTUAL METHODS */
    queue_push_fn_t push;
    queue_pop_fn_t pop;
    queue_peek_fn_t peek;
    queue_is_empty_fn_t is_empty;

    /* FIELDS */
    size_t size;
} queue_base_t;

#endif // COMMON_H__
