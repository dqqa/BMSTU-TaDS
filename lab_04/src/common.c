#define _GNU_SOURCE
#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

math_op_ex_t operations[] = {
    {MATH_ADD, 1, '+'},
    {MATH_SUB, 1, '-'},
    {MATH_MUL, 2, '*'},
    {MATH_DIV, 2, '/'},
    {MATH_OPEN_PAREN, 3, '('},
    {MATH_CLOSE_PAREN, 3, ')'},
    {MATH_UNKNOWN, 999, '!'}};

math_op_ex_t operation_parse(char c)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        if (c == operations[i].symbol)
            return operations[i];
    }
    return operations[ARR_LEN(operations) - 1];
}

math_op_ex_t get_operation_ex(math_op_t op)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
    {
        if (op == operations[i].op)
            return operations[i];
    }
    return operations[ARR_LEN(operations) - 1];
}
