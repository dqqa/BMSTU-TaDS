#ifndef TREE_H__
#define TREE_H__

#include <stdio.h>
#include <stdbool.h>

typedef char *data_t;

typedef struct __tree_t
{
    data_t data;
    bool is_repeated;
    struct __tree_t *lhs, *rhs;
} tree_t;

typedef void (*tree_apply_fn_t)(tree_t *subtree, void *arg);

int tree_create(tree_t **t, const char *data);
void tree_free(tree_t *tree);

void tree_apply_pre(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);
void tree_apply_in(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);
void tree_apply_post(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);

tree_t *tree_insert_node(tree_t *tree, tree_t *src, int *err);
int tree_insert_str(tree_t **tree, const char *src);

int tree_remove(tree_t **tree, tree_t *what);
int tree_remove_str(tree_t **tree, const char *what);

void tree_balance(tree_t **tree);

tree_t *tree_search(tree_t *tree, const char *data);
void tree_search_symbol(tree_t *tree, char symbol, size_t *cnt);

void tree_repeat_reset(tree_t *tree);
void tree_to_graphviz(FILE *fp, const char *tree_name, tree_t *t);

int tree_save_tmp_open(tree_t *t);

#endif // TREE_H__
