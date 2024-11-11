#include "errors.h"
#include "file_reader.h"
#include "tree.h"
#include <stdio.h>

#define return_defer(num) \
    do                    \
    {                     \
        result = (num);   \
        goto defer;       \
    } while (0)

void tree_printer(tree_t *t, void *fmt)
{
    printf(fmt, t->data);
}

int main(void)
{
    tree_t *t = NULL;
    int rc = ERR_OK, result = ERR_OK;

    rc = file_load_ex("tests/1.txt", &t);
    if (rc != ERR_OK)
        return_defer(rc);

    tree_to_graphviz(stdout, "tree", t);

    printf("tree_search_symbol:\n");
    size_t cnt = 0;
    tree_search_symbol(t, '0', &cnt);
    printf("COUNT: %zu\n\n", cnt);

    printf("file_search_symbol_ex:\n");
    cnt = 0;
    file_search_symbol_ex("tests/1.txt", '0', &cnt);
    printf("COUNT: %zu\n\n", cnt);

    defer:
    tree_free(t);

    return result;
}
