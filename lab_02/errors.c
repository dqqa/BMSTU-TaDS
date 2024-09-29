#include "errors.h"
#include <stdio.h>

void print_err(int err)
{
    switch (err)
    {
        case ERR_IO:
            printf("IO error");
            break;
        case ERR_OVERFLOW:
            printf("Overflow error");
            break;
        case ERR_UNKNOWN_OP:
            printf("Unknown operation error");
            break;
        case ERR_UNKNOWN_TYPE:
            printf("Unknown type error");
            break;
        default:
            printf("Unknown error: %d", err);
            break;
    }
    printf("\n");
}
