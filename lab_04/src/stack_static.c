#include "stack_static.h"
#include "common.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>

int stack_static_op_create(stack_op_static_t *s)
{
    memset(s->ops, 0, sizeof(s->ops));
    s->top = 0;
    return ERR_OK;
}

int stack_static_op_push(stack_op_static_t *s, math_op_t el)
{
    if (s->top == MAX_STACK_SIZE - 1)
        return ERR_STACK_OVERFLOW;

    s->ops[s->top++] = el;
    return ERR_OK;
}

int stack_static_op_pop(stack_op_static_t *s, math_op_t *el)
{
    if (s->top == 0)
        return ERR_STACK_UNDERFLOW;

    *el = s->ops[--s->top];
    return ERR_OK;
}
