#include "rev_polish_notation.h"
#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func(node_t *node, void *fmt)
{
    printf(fmt, ((math_op_ex_t *)node->data)->symbol);
}

static int RPN_print_until_opening(stack_dyn_t *s)
{
    math_op_ex_t cur_op_ex;
    bool need_continue = true;
    while (need_continue)
    {
        int rc = stack_dyn_pop(s, &cur_op_ex, sizeof(cur_op_ex));
        if (rc != ERR_OK)
            return rc;

        if (rc == ERR_OK)
        {
            if (cur_op_ex.op != MATH_OPEN_PAREN)
                printf("%c ", cur_op_ex.symbol);
            else
                need_continue = false;
        }
    }
    return ERR_OK;
}

int RPN_parse_expr(const char *expr)
{
    stack_dyn_t s;
    stack_dyn_create(&s);
    int rc = ERR_OK;
    size_t num_cnt = 0, op_cnt = 0, open_p_cnt = 0, close_p_cnt = 0;
    while (*expr)
    {
        data_t num;
        int nread;
        if (sscanf(expr, "%" DATA_SCN "%n", &num, &nread) == 1)
        {
            printf("%" DATA_PRI " ", num); // 1.
            expr += nread;
            num_cnt++;
        }
        else
        {
            math_op_ex_t cur_op_ex = operation_parse(*expr);
            if (cur_op_ex.op == MATH_UNKNOWN)
            {
                rc = ERR_IO;
                goto err;
            }

            if (cur_op_ex.op == MATH_CLOSE_PAREN)
                close_p_cnt++;
            else if (cur_op_ex.op == MATH_OPEN_PAREN)
                open_p_cnt++;
            else
                op_cnt++;

            if (cur_op_ex.op == MATH_CLOSE_PAREN)
                RPN_print_until_opening(&s); // 3.
            else
            {
                math_op_ex_t prev_op_ex;
                if (stack_dyn_peek(&s, &prev_op_ex, sizeof(prev_op_ex)) == ERR_OK)
                {
                    if (prev_op_ex.op == MATH_OPEN_PAREN || prev_op_ex.priority < cur_op_ex.priority)
                        stack_dyn_push(&s, &cur_op_ex, sizeof(cur_op_ex)); // 4.b || 4.c
                    else if (prev_op_ex.priority >= cur_op_ex.priority)
                    {
                        printf("%c ", prev_op_ex.symbol); // 4.a
                        stack_dyn_pop(&s, NULL, 0);
                        stack_dyn_push(&s, &cur_op_ex, sizeof(cur_op_ex));
                    }
                }
                else
                    stack_dyn_push(&s, &cur_op_ex, sizeof(cur_op_ex)); // 5.
            }
            expr++;
        }
        // skip all spaces
        expr += strspn(expr, " \n\t");
    }

    // expr ended, need to print contents of stack
    stack_dyn_apply(&s, func, "%c ");

    err:
    stack_dyn_free(&s);
    if (rc != ERR_OK)
        return rc;

    printf("\n");
    if (num_cnt && num_cnt - 1 == op_cnt && close_p_cnt == open_p_cnt)
        return ERR_OK;
    else
        return ERR_INVALID_EXPR;
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

    printf("Выражение в постфиксной форме: ");
    if ((rc = RPN_parse_expr(line)) != ERR_OK)
        goto cleanup;

    cleanup:
    if (line)
        free(line);

    return rc;
}
