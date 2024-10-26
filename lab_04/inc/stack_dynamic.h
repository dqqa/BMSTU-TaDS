#ifndef STACK_DYNAMIC_H__
#define STACK_DYNAMIC_H__

#include "common.h"
#include <stddef.h>

typedef struct __node_t
{
    // math_op_ex_t op;
    void *data;
    struct __node_t *next;
} node_t;

typedef struct __stack_t
{
    size_t size;
    node_t *head;
} stack_dyn_t;

typedef void (*stack_apply_fn)(node_t *node, void *arg);

int stack_dyn_create(stack_dyn_t *s);

void stack_dyn_free(stack_dyn_t *s);

int stack_dyn_push(stack_dyn_t *s, const void *src, size_t src_size);

void stack_dyn_apply(stack_dyn_t *s, stack_apply_fn func, void *arg);
int stack_dyn_peek(stack_dyn_t *s, void *dst, size_t dst_size);

int stack_dyn_pop(stack_dyn_t *s, void *dst, size_t dst_size);

#endif // STACK_DYNAMIC_H__
