#ifndef TREE_H__
#define TREE_H__

#include "common.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct __bst_tree_t
{
    data_t data;
    bool is_repeated;
    struct __bst_tree_t *lhs, *rhs;
} bst_tree_t;

typedef void (*bst_apply_fn_t)(bst_tree_t *subtree, void *arg);

int bst_load_file(FILE *fp, bst_tree_t **tree);
int bst_load_file_ex(const char *filename, bst_tree_t **tree);

int bst_create(bst_tree_t **t, const char *data);
void bst_free(bst_tree_t *tree);

void bst_apply_pre(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg);
void bst_apply_in(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg);
void bst_apply_post(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg);

bst_tree_t *bst_insert_node(bst_tree_t *tree, bst_tree_t *src, int *err);
int bst_insert_str(bst_tree_t **tree, const char *src);

int bst_remove(bst_tree_t **tree, bst_tree_t *what);
int bst_remove_str(bst_tree_t **tree, const char *what);

int bst_remove_nodes_starting_with(bst_tree_t **tree, char c);

void bst_balance(bst_tree_t **tree);

bst_tree_t *bst_search(bst_tree_t *tree, const char *data);
void bst_search_symbol(bst_tree_t *tree, char symbol, size_t *cnt);

void bst_repeat_reset(bst_tree_t *tree);
void bst_to_graphviz(FILE *fp, const char *tree_name, bst_tree_t *t);

int bst_save_tmp_open(bst_tree_t *t);

#endif // TREE_H__
