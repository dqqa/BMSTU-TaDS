#ifndef QUEUE_ARR_H__
#define QUEUE_ARR_H__

#include "common.h"
#include <stdio.h>

#define MAX_QUEUE_SIZE 128

typedef struct __queue_arr_t
{
    data_t data[MAX_QUEUE_SIZE];
    size_t size;
} queue_arr_t;

#endif // QUEUE_ARR_H__
