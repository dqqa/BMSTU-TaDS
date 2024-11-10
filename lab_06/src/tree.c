#include "tree.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) ((void)(x))

int tree_create(tree_t **t)
{
    tree_t *tree = malloc(sizeof(*tree));
    if (!tree)
        return ERR_ALLOC;

    tree->lhs = NULL;
    tree->rhs = NULL;
    tree->data = NULL;

    *t = tree;

    return ERR_OK;
}

void tree_free(tree_t *tree)
{
    if (!tree)
        return;

    tree_free(tree->lhs);
    tree_free(tree->rhs);

    free(tree->data);
    free(tree);
}

int tree_apply(const tree_t *tree, tree_apply_fn_t apply_fn, void *arg);

int tree_insert(tree_t **tree, const data_t *src)
{
    UNUSED(src);
    // Recursive search for a place to insert
    int rc = ERR_OK;
    if (!*tree)
    {
        rc = tree_create(tree);
        if (rc != ERR_OK)
            return rc;
    }

    return ERR_OK;
}

int tree_remove(tree_t *tree, data_t **dst);
int tree_search(const tree_t *tree, const data_t *data);
