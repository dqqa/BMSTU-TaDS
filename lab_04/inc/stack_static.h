#ifndef STACK_STATIC_H__
#define STACK_STATIC_H__

#include "common.h"
#include <stddef.h>

#define MAX_STACK_SIZE 100

typedef struct __stack_data_static_t
{
    data_t data[MAX_STACK_SIZE];
    size_t top;
} stack_data_static_t;

typedef struct __stack_op_static_t
{
    math_op_t ops[MAX_STACK_SIZE];
    size_t top;
} stack_op_static_t;

int stack_static_data_create(stack_data_static_t *s);
int stack_static_op_create(stack_op_static_t *s);

int stack_static_data_push(stack_data_static_t *s, data_t el);
int stack_static_op_push(stack_op_static_t *s, math_op_t el);

int stack_static_data_pop(stack_data_static_t *s, data_t *el);
int stack_static_op_pop(stack_op_static_t *s, math_op_t *el);

#endif // STACK_STATIC_H__
