#include "errors.h"
#include "tree.h"
#include <stdio.h>

#define return_defer(num) \
    do                    \
    {                     \
        result = (num);   \
        goto cleanup;     \
    } while (0)

void tree_printer(tree_t *t, void *fmt)
{
    printf(fmt, t->data);
}

int main(void)
{
    tree_t *t = NULL;
    int rc = ERR_OK, result = ERR_OK;

    if ((rc = tree_insert_str(&t, "123")) != ERR_OK)
        return_defer(rc);

    if ((rc = tree_insert_str(&t, "234")) != ERR_OK)
        return_defer(rc);

    if ((rc = tree_insert_str(&t, "234")) != ERR_OK)
        return_defer(rc);

    if ((rc = tree_insert_str(&t, "0")) != ERR_OK)
        return_defer(rc);

    printf("TREE:\n");
    tree_apply_pre(t, tree_printer, "%s, ");
    printf("\n");
    tree_apply_in(t, tree_printer, "%s, ");
    printf("\n");
    tree_apply_post(t, tree_printer, "%s, ");
    printf("\n");

    cleanup:
    tree_free(t);
    return result;
}
