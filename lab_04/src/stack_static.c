#include "stack_static.h"
#include "common.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>

int stack_static_create(stack_static_t *s)
{
    memset(s->data, 0, sizeof(s->data));
    s->top = -1;
    return ERR_OK;
}

int stack_static_push(stack_static_t *s, int el)
{
    if (s->top == MAX_STACK_SIZE - 1)
        return ERR_STACK_OVERFLOW;

    s->data[++s->top] = el;
    return ERR_OK;
}

int stack_static_pop(stack_static_t *s, int *el)
{
    if (s->top == -1)
        return ERR_STACK_UNDERFLOW;

    *el = s->data[s->top--];
    return ERR_OK;
}

void stack_static_apply(stack_static_t *s, stack_static_apply_fn func, void *arg)
{
    for (long t = s->top; t > -1; t--)
        func(s->data + t, arg);
}
