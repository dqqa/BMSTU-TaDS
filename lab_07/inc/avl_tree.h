#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include "errors.h"

typedef struct node node_t;

struct node
{
    node_t *lhs, *rhs;
    char height;

    const char *key;
    int value;
};

typedef void (*avl_apply_fn_t)(const char *key, int *num, void *param);

node_t *node_create(void);

int avl_insert(node_t **head, node_t *new_node);
void avl_free(node_t **head);

const node_t *avl_search(const node_t *head, const char *key);
node_t *avl_remove(node_t *head, const char *key);
void avl_apply(node_t *head, avl_apply_fn_t func, void *param);

const node_t *avl_find_min(const node_t *head);
const node_t *avl_find_max(const node_t *head);

node_t *avl_node_balance(node_t *head);

char avl_get_height(const node_t *head);
int avl_calc_balance_factor(const node_t *head);

node_t *avl_rotate_left(node_t *head);
node_t *avl_rotate_right(node_t *head);

void avl_fix_height(node_t *head);

#endif // AVL_TREE_H__
