#include "rev_polish_notation.h"
#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static math_op_t operation_parse(char c)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        if (c == operations[i].symbol)
            return (math_op_t)i;
    }
    return MATH_UNKNOWN;
}

static math_op_ex_t operation_ex(math_op_t op)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        if (op == operations[i].op)
            return operations[i];
    }
    return operations[6]; // TODO
}

void func(node_op_t *node, void *fmt)
{
    printf(fmt, operation_ex(node->op).symbol);
}

static int RPN_print_until_opening(stack_op_dyn_t *s)
{
    math_op_t op;
    bool need_continue = true;
    while (need_continue)
    {
        int rc = stack_dyn_op_pop(s, &op);
        if (rc != ERR_OK)
            return rc;

        if (rc == ERR_OK)
        {
            if (op != MATH_OPEN_PAREN)
                printf("%c ", operation_ex(op).symbol);
            else
                need_continue = false;
        }
    }
    return ERR_OK;
}

int RPN_parse_expr(const char *expr)
{
    stack_op_dyn_t s;
    stack_dyn_op_create(&s);

    while (*expr)
    {
        data_t num;
        int nread;
        if (sscanf(expr, "%" DATA_SCN "%n", &num, &nread) == 1)
        {
            printf("%" DATA_PRI " ", num); // 1.
            expr += nread;
        }
        else
        {
            math_op_t op = operation_parse(*expr);
            if (op == MATH_UNKNOWN)
                return ERR_IO;

            if (op == MATH_CLOSE_PAREN)
                RPN_print_until_opening(&s); // 3.
            else
            {
                math_op_t prev_op;
                math_op_ex_t cur_op_ex = operation_ex(op);
                if (stack_dyn_op_peek(&s, &prev_op) == ERR_OK)
                {
                    math_op_ex_t prev_op_ex = operation_ex(prev_op);
                    if (prev_op_ex.op == MATH_OPEN_PAREN || prev_op_ex.priority < cur_op_ex.priority)
                        stack_dyn_op_push(&s, op); // 4.b || 4.c
                    else if (prev_op_ex.priority >= cur_op_ex.priority)
                    {
                        printf("%c ", prev_op_ex.symbol);
                        stack_dyn_op_pop(&s, NULL);
                        stack_dyn_op_push(&s, op);
                    }
                }
                else
                    stack_dyn_op_push(&s, op); // 5.
            }
            expr++;
        }
        // skip all spaces
        expr += strspn(expr, " \n\t");
    }

    // expr ended, need to print contents of stack
    stack_dyn_op_apply(&s, func, "%c ");
    stack_dyn_op_free(&s);

    printf("\n");
    return ERR_OK;
}

int RPN_parse_expr_ex(FILE *fp)
{
    int rc = ERR_OK;
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t n = getline(&line, &bufsize, fp);
    (void)n;

    if (!line)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }
    printf("Orig:   %s", line);
    printf("Parsed: ");
    if ((rc = RPN_parse_expr(line)) != ERR_OK)
        goto cleanup;

    cleanup:
    if (line)
        free(line);

    return rc;
}
