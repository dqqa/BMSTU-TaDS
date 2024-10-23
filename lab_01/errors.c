#include "errors.h"
#include <stdio.h>

void print_err(int err)
{
    switch (err)
    {
        case ERR_IO:
            printf("IO error");
            break;
        case ERR_DIVIDE_BY_ZERO:
            printf("Divide by zero error");
            break;
        case ERR_OVERFLOW:
            printf("Overflow error");
            break;
        case ERR_UNDERFLOW:
            printf("Underflow error");
            break;
        case ERR_NUMBER:
            printf("Bad number");
            break;
        default:
            printf("Unknown error: %d", err);
            break;
    }
    printf("\n");
}
