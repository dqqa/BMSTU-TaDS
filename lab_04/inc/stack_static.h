#ifndef STACK_STATIC_H__
#define STACK_STATIC_H__

#include "common.h"
#include <stddef.h>

#define MAX_STACK_SIZE 100

typedef struct __stack_static_t
{
    // TODO: implement some universal? container
    int data[MAX_STACK_SIZE];
    long top;
} stack_static_t;

typedef void (*stack_static_apply_fn)(int *data, void *arg);

int stack_static_create(stack_static_t *s);

int stack_static_push(stack_static_t *s, int el);

int stack_static_pop(stack_static_t *s, int *el);

void stack_static_apply(stack_static_t *s, stack_static_apply_fn func, void *arg);

#endif // STACK_STATIC_H__
