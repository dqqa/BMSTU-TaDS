#include "errors.h"
#include <stdio.h>

void print_err(int rc)
{
    switch (rc)
    {
        case ERR_IO:
            printf("IO error");
            break;
        case ERR_ALLOC:
            printf("Allocation error");
            break;
        case ERR_RANGE:
            printf("Range error");
            break;
        case ERR_PARAMS:
            printf("Params error");
            break;
        default:
            printf("Unknown error: %d", rc);
            break;
    }
    printf("\n");
}
