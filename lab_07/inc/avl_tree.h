#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include "errors.h"

typedef struct node avl_node_t;

struct node
{
    avl_node_t *lhs, *rhs;
    char height;

    const char *key;
    int value;
};

typedef void (*avl_apply_fn_t)(const char *key, int *num, void *param);

avl_node_t *avl_node_create(void);

int avl_insert(avl_node_t **head, avl_node_t *new_node);
void avl_free(avl_node_t **head);

const avl_node_t *avl_search(const avl_node_t *head, const char *key);
avl_node_t *avl_remove(avl_node_t *head, const char *key);

void avl_apply_pre(avl_node_t *head, avl_apply_fn_t func, void *param);
void avl_apply_in(avl_node_t *head, avl_apply_fn_t func, void *param);
void avl_apply_post(avl_node_t *head, avl_apply_fn_t func, void *param);

const avl_node_t *avl_find_min(const avl_node_t *head);
const avl_node_t *avl_find_max(const avl_node_t *head);

avl_node_t *avl_node_balance(avl_node_t *head);

char avl_get_height(const avl_node_t *head);
int avl_calc_balance_factor(const avl_node_t *head);

avl_node_t *avl_rotate_left(avl_node_t *head);
avl_node_t *avl_rotate_right(avl_node_t *head);

void avl_fix_height(avl_node_t *head);

#endif // AVL_TREE_H__
