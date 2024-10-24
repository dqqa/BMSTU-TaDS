#define _GNU_SOURCE
#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

match_t op_matches[] = {
    {MATH_ADD, 1, "+"},
    {MATH_SUB, 1, "-"},
    {MATH_MUL, 2, "*"},
    {MATH_DIV, 2, "/"},
    {MATH_OPEN_PAREN, 3, "("},
    {MATH_CLOSE_PAREN, 3, ")"}};

static math_op_t operation_parse(const char *s)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        char buf[2] = {0};
        buf[0] = *s;
        if (strcmp(buf, op_matches[i].str) == 0)
            return (math_op_t)i;
    }
    return MATH_UNKNOWN;
}

static const char *operation_str(math_op_t op)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        if (op == op_matches[i].op)
            return op_matches[i].str;
    }
    return NULL;
}

int RPN_parse_expr(const char *expr)
{
    stack_op_dyn_t s;
    //stack_dyn_op_create(&s);

    while (*expr)
    {
        data_t num;
        int nread;
        if (sscanf(expr, "%" DATA_SCN "%n", &num, &nread) == 1)
        {
            printf("%" DATA_PRI " ", num);
            expr += nread;
        }
        else
        {
            math_op_t op = operation_parse(expr);
            printf("%s ", operation_str(op));
            expr++;
            //stack_dyn_op_push(&s, op);
        }
        // skip all spaces
        expr += strspn(expr, " \n\t");
    }
    return ERR_OK;
}

int RPN_parse_expr_ex(FILE *fp)
{
    int rc = ERR_OK;
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t n = getline(&line, &bufsize, fp);

    if (!line)
    {
        rc = ERR_ALLOC;
        goto cleanup;
    }
    if ((rc = RPN_parse_expr(line)) != ERR_OK)
        goto cleanup;

    cleanup:
    if (line)
        free(line);

    return rc;
}
