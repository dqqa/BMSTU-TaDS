#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include "errors.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct __avl_node avl_tree_t;

struct __avl_node
{
    avl_tree_t *lhs, *rhs;
    int height;

    char *key;
    bool is_repeated;
};

typedef void (*avl_apply_fn_t)(avl_tree_t *node, void *param);

int avl_load_file(FILE *fp, avl_tree_t **tree);
int avl_load_file_ex(const char *filename, avl_tree_t **tree);

avl_tree_t *avl_create(const char *data);
void avl_free(avl_tree_t **head);

int avl_insert_node(avl_tree_t **head, avl_tree_t *new_node);
int avl_insert_str(avl_tree_t **head, const char *key);

avl_tree_t *avl_search(avl_tree_t *head, const char *key, size_t *cmps);
avl_tree_t *avl_remove(avl_tree_t *head, const char *key);

int avl_remove_nodes_starting_with(avl_tree_t **tree, char c);

void avl_apply_pre(avl_tree_t *head, avl_apply_fn_t func, void *param);
void avl_apply_in(avl_tree_t *head, avl_apply_fn_t func, void *param);
void avl_apply_post(avl_tree_t *head, avl_apply_fn_t func, void *param);

const avl_tree_t *avl_find_min(const avl_tree_t *head);
const avl_tree_t *avl_find_max(const avl_tree_t *head);

avl_tree_t *avl_node_balance(avl_tree_t *head);

int avl_get_height(const avl_tree_t *head);
int avl_calc_balance_factor(const avl_tree_t *head);

avl_tree_t *avl_rotate_left(avl_tree_t *head);
avl_tree_t *avl_rotate_right(avl_tree_t *head);

void avl_fix_height(avl_tree_t *head);

void avl_to_graphviz(FILE *fp, const char *tree_name, avl_tree_t *t);
int avl_save_tmp_open(avl_tree_t *t);

void avl_repeat_reset(avl_tree_t *tree);
void avl_search_symbol(avl_tree_t *tree, char symbol, size_t *cnt);

float avl_calc_avg_cmp(avl_tree_t *root);
void avl_calc_ram_usage(avl_tree_t *tree, size_t *bytes);

#endif // AVL_TREE_H__
