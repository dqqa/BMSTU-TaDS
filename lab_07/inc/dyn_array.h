#ifndef DYN_ARRAY_H__
#define DYN_ARRAY_H__

#include "errors.h"
#include <stdlib.h>

#define DYN_ARR_INIT_CAP 16

#define dyn_arr_append(arr, item) ({                                             \
    int rc = ERR_OK;                                                             \
    if ((arr).count >= (arr).capacity)                                           \
    {                                                                            \
        (arr).capacity = (arr).capacity ? (arr).capacity * 2 : DYN_ARR_INIT_CAP; \
        typeof((arr).items) newitems = realloc((arr).items, (arr).capacity);     \
        if (newitems == NULL)                                                    \
        {                                                                        \
            rc = ERR_ALLOC;                                                      \
            goto err;                                                            \
        }                                                                        \
        (arr).items = newitems;                                                  \
    }                                                                            \
    (arr).items[(arr).count++] = (item);                                         \
    err:                                                                         \
    rc;                                                                          \
})

#define dyn_arr_free(arr)   \
    do                      \
    {                       \
        free((arr).items);  \
        (arr).items = NULL; \
        (arr).count = 0;    \
        (arr).capacity = 0; \
    } while (0)

#endif // DYN_ARRAY_H__
