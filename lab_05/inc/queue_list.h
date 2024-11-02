#ifndef QUEUE_ARR_H__
#define QUEUE_ARR_H__

#include "common.h"

typedef struct __node_t
{
    void *data;
    struct __node_t *next;
} node_t;

typedef struct __queue_list
{
    node_t *head;
    node_t *end;
} queue_list_t;

#endif // QUEUE_ARR_H__
