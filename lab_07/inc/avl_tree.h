#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include "errors.h"
#include <stdbool.h>

typedef struct __avl_node avl_tree_t;

struct __avl_node
{
    avl_tree_t *lhs, *rhs;
    char height;

    const char *key;
    bool is_repeated;
};

typedef void (*avl_apply_fn_t)(avl_tree_t *node, void *param);

int avl_create(avl_tree_t **t, const char *data);
void avl_free(avl_tree_t **head);

int avl_insert_node(avl_tree_t **head, avl_tree_t *new_node);
int avl_insert_str(avl_tree_t **head, const char *src);

const avl_tree_t *avl_search(const avl_tree_t *head, const char *key);
avl_tree_t *avl_remove(avl_tree_t *head, const char *key);

void avl_apply_pre(avl_tree_t *head, avl_apply_fn_t func, void *param);
void avl_apply_in(avl_tree_t *head, avl_apply_fn_t func, void *param);
void avl_apply_post(avl_tree_t *head, avl_apply_fn_t func, void *param);

const avl_tree_t *avl_find_min(const avl_tree_t *head);
const avl_tree_t *avl_find_max(const avl_tree_t *head);

avl_tree_t *avl_node_balance(avl_tree_t *head);

char avl_get_height(const avl_tree_t *head);
int avl_calc_balance_factor(const avl_tree_t *head);

avl_tree_t *avl_rotate_left(avl_tree_t *head);
avl_tree_t *avl_rotate_right(avl_tree_t *head);

void avl_fix_height(avl_tree_t *head);

#endif // AVL_TREE_H__
