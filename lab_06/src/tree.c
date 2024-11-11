#define _GNU_SOURCE

#include "tree.h"
#include "errors.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) ((void)(x))
#define NOTHING
#define UNREACHABLE(msg)        \
    do                          \
    {                           \
        fprintf(stderr, (msg)); \
        exit(1);                \
    } while (0)

int tree_create(tree_t **t, const char *data)
{
    int rc = ERR_OK;
    *t = NULL;

    tree_t *tree = malloc(sizeof(*tree));
    if (!tree)
        return ERR_ALLOC;

    tree->lhs = NULL;
    tree->rhs = NULL;
    tree->data = NULL;

    tree->data = strdup(data);
    if (!tree->data)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    err:
    if (rc != ERR_OK)
        free(tree);
    else
        *t = tree;

    return rc;
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

void tree_apply_pre(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    apply_fn(tree, arg);
    tree_apply_pre(tree->lhs, apply_fn, arg);
    tree_apply_pre(tree->rhs, apply_fn, arg);
}

void tree_apply_in(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    tree_apply_in(tree->lhs, apply_fn, arg);
    apply_fn(tree, arg);
    tree_apply_in(tree->rhs, apply_fn, arg);
}

void tree_apply_post(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    tree_apply_post(tree->lhs, apply_fn, arg);
    tree_apply_post(tree->rhs, apply_fn, arg);
    apply_fn(tree, arg);
}

tree_t *tree_insert_node(tree_t *tree, tree_t *src, int *err)
{
    if (!tree)
        return src;

    *err = ERR_OK;

    int cmp_res = strcmp(tree->data, src->data);
    if (cmp_res == 0)
        *err = ERR_REPEAT; // assert(0 && "Yet there is no support of repeating nodes!");
    else if (cmp_res < 0)
        tree->rhs = tree_insert_node(tree->rhs, src, err);
    else
        tree->lhs = tree_insert_node(tree->lhs, src, err);

    return tree;
}

int tree_insert_str(tree_t **tree, const char *src)
{
    tree_t *subtree;
    int rc = tree_create(&subtree, src);
    if (rc != ERR_OK)
        return rc;

    tree_t *newtree = tree_insert_node(*tree, subtree, &rc);
    if (rc != ERR_OK)
        goto err;

    *tree = newtree;

    err:
    if (rc != ERR_OK)
        tree_free(subtree);

    return rc;
}

void tree_remove(tree_t *tree, tree_t *what);

tree_t *tree_search(tree_t *tree, const char *data)
{
    if (!tree)
        return NULL;

    int cmp_res = strcmp(data, tree->data);
    if (cmp_res == 0)
        return tree;
    else if (cmp_res < 0)
        return tree_search(tree->lhs, data);
    else
        return tree_search(tree->rhs, data);

    UNREACHABLE("tree_search");
}

void tree_search_symbol(const tree_t *tree, char symbol, size_t *cnt)
{
    if (!tree)
        return;

    if (tree->data[0] == symbol)
    {
        printf("%s\n", tree->data);
        if (cnt)
            *cnt += 1;

        tree_search_symbol(tree->lhs, symbol, cnt);
        tree_search_symbol(tree->rhs, symbol, cnt);
    }
    else if (tree->data[0] < symbol)
        tree_search_symbol(tree->rhs, symbol, cnt);
    else
        tree_search_symbol(tree->lhs, symbol, cnt);
}

static void to_dot(tree_t *tree, void *fp)
{
    static int null_cnt = 0;

    if (tree->lhs)
        fprintf(fp, "  %s -> %s;\n", tree->data, tree->lhs->data);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->data, null_cnt);
        fprintf(fp, "  null_%d [color=\"red\"];\n", null_cnt);
        null_cnt++;
    }

    if (tree->rhs)
        fprintf(fp, "  %s -> %s;\n", tree->data, tree->rhs->data);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->data, null_cnt);
        fprintf(fp, "  null_%d [color=\"red\"];\n", null_cnt);
        null_cnt++;
    }
}

void tree_to_graphviz(FILE *fp, const char *tree_name, tree_t *t)
{
    fprintf(fp, "digraph %s {\n", tree_name);
    tree_apply_pre(t, to_dot, fp);
    fprintf(fp, "}\n");
}
