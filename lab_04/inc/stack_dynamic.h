#ifndef STACK_DYNAMIC_H__
#define STACK_DYNAMIC_H__

#include "common.h"
#include <stddef.h>

typedef struct __node_data_t
{
    data_t data;
    struct __node_data_t *next;
} node_data_t;

typedef struct __node_op_t
{
    math_op_t op;
    struct __node_op_t *next;
} node_op_t;

typedef struct __stack_data_t
{
    size_t size;
    node_data_t *head;
} stack_data_dyn_t;

typedef struct __stack_op_t
{
    size_t size;
    node_op_t *head;
} stack_op_dyn_t;

int stack_dyn_data_create(stack_data_dyn_t *s);
int stack_dyn_op_create(stack_op_dyn_t *s);

int stack_dyn_data_push(stack_data_dyn_t *s, data_t el);
int stack_dyn_op_push(stack_op_dyn_t *s, math_op_t el);

int stack_dyn_data_pop(stack_data_dyn_t *s, data_t *el);
int stack_dyn_op_pop(stack_op_dyn_t *s, math_op_t *el);

#endif // STACK_DYNAMIC_H__
