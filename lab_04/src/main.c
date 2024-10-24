#include "common.h"
#include "errors.h"
#include "stack_dynamic.h"
#include "stack_static.h"
#include <stdio.h>

int main(void)
{
    for (size_t i = 0; i < MATH_COUNT; i++)
        printf("%s ", op_matches[i].str);
    return ERR_OK;
}
