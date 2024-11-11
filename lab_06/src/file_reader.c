#define _GNU_SOURCE

#include "file_reader.h"
#include "errors.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>

static char *get_str(FILE *fp, long *size)
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

int file_load(FILE *fp, tree_t **tree)
{
    *tree = NULL;
    int rc = ERR_OK;

    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_IO;
    while (line)
    {
        rc = tree_insert_str(tree, line);
        if (rc != ERR_REPEAT && rc != ERR_OK) // TODO
            goto err;

        free(line);
        line = get_str(fp, NULL);
    }

    err:
    if (rc != ERR_REPEAT && rc != ERR_OK)
        tree_free(*tree);

    return rc;
}

int file_load_ex(const char *filename, tree_t **tree)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = file_load(fp, tree);

    fclose(fp);
    return rc;
}

int file_search_symbol(FILE *fp, char symbol, size_t *cnt)
{
    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_ALLOC;

    while (line)
    {
        if (line[0] == symbol)
        {
            // printf("%s\n", line);
            if (cnt)
                *cnt += 1;
        }

        free(line);
        line = get_str(fp, NULL);
    }

    return ERR_OK;
}

int file_search_symbol_ex(const char *filename, char symbol, size_t *cnt)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = file_search_symbol(fp, symbol, cnt);

    fclose(fp);
    return rc;
}
