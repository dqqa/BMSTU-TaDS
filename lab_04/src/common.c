#define _GNU_SOURCE
#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

math_op_ex_t operations[] = {
    {MATH_ADD, 1, '+'},
    {MATH_SUB, 1, '-'},
    {MATH_MUL, 2, '*'},
    {MATH_DIV, 2, '/'},
    {MATH_OPEN_PAREN, 3, '('},
    {MATH_CLOSE_PAREN, 3, ')'},
    {MATH_UNKNOWN, 999, '!'}};
