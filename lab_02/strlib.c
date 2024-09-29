#include "strlib.h"
#include "errors.h"
#include <string.h>

int str_input(FILE *fp, char *buf, size_t buf_size)
{
    size_t length = 0;
    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (length > buf_size)
            return ERR_OVERFLOW;
        if (c == '/' || c == '\n')
        {
            buf[length] = 0;
            return ERR_OK;
        }
        buf[length] = c;
        length++;
    }
    if (c == EOF)
        return c;

    return ERR_OK;
}
