#include "str.h"
#include <stdio.h>
#include <stdlib.h>

char *get_str(FILE *fp, long *size)
{
    char *line = NULL;
    size_t line_size = 0;
    ssize_t n = getline(&line, &line_size, fp);
    if (n < 0)
    {
        free(line);
        return NULL;
    }
    if (size)
        *size = n;

    if (n && line[n - 1] == '\n')
        line[n - 1] = 0;

    return line;
}
