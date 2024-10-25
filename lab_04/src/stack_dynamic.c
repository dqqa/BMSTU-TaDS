#include "stack_dynamic.h"
#include "common.h"
#include "errors.h"
#include <stdlib.h>

// void stack_dyn_data_create(stack_data_dyn_t *s)
// {
//     s->head=NULL;
//     s->size=0;
// }

int stack_dyn_op_create(stack_op_dyn_t *s)
{
    s->head = NULL;
    s->size = 0;

    return ERR_OK;
}

int stack_dyn_data_push(stack_data_dyn_t *s, data_t el);

int stack_dyn_op_push(stack_op_dyn_t *s, math_op_t el)
{
    node_op_t *new_node = malloc(sizeof(*new_node));
    if (!new_node)
        return ERR_ALLOC;

    new_node->next = s->head;
    new_node->op = el;

    s->size++;
    s->head = new_node;

    return ERR_OK;
}

int stack_dyn_data_pop(stack_data_dyn_t *s, data_t *el);

int stack_dyn_op_pop(stack_op_dyn_t *s, math_op_t *el)
{
    if (s->size == 0)
        return ERR_STACK_UNDERFLOW;

    node_op_t *node = s->head;
    
    if (el)
        *el = s->head->op;
    s->head = s->head->next;
    s->size--;

    free(node);
    return ERR_OK;
}

void stack_dyn_op_free(stack_op_dyn_t *s)
{
    while (s->head)
    {
        node_op_t *node = s->head;
        s->head = s->head->next;
        free(node);
    }
}

void stack_dyn_op_apply(stack_op_dyn_t *s, stack_apply_fn func, void *arg)
{
    while (s->head)
    {
        func(s->head, arg);
        s->head = s->head->next;
    }
}

int stack_dyn_op_peek(stack_op_dyn_t *s, math_op_t *el)
{
    if (!s->head)
        return ERR_STACK_UNDERFLOW;

    if (el)
        *el = s->head->op;

    return ERR_OK;
}
