#include "str.h"
#include "constants.h"
#include "errors.h"
#include <stdio.h>
#include <string.h>

int str_input(FILE *fp, char *buf, size_t max_buf_size)
{
    if (!fgets(buf, max_buf_size, fp))
        return ERR_IO;

    char *newline = strrchr(buf, '\n');
    if (newline)
        *newline = 0;
    else
        return ERR_OVERFLOW;

    return ERR_OK;
}

size_t str_strip(char *str)
{
    size_t length = strlen(str);
    if (!length)
        return length;

    size_t init_length = strspn(str, SPACE_SYMBOLS);
    size_t payload_length = strcspn(str + init_length, SPACE_SYMBOLS);

    for (size_t i = 0; i < init_length + payload_length; i++)
        str[i] = str[i + init_length];
    str[payload_length] = 0;

    return payload_length;
}

//                 123132qweqee.1e12ee33                      
