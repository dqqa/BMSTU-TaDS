#ifndef LINKED_LIST_H__
#define LINKED_LIST_H__

#include "errors.h"
#include "node.h"

typedef void (*list_apply_fn_t)(const char *key, int *num, void *param);

void list_free(avl_tree_t **head);

int push_back(avl_tree_t **head, const char *key, int val);

void list_apply(avl_tree_t *head, list_apply_fn_t func, void *arg);

const avl_tree_t *list_search_by_key(const avl_tree_t *head, const char *key);

int list_remove_by_key(avl_tree_t **head, const char *key);

const avl_tree_t *list_find_min(const avl_tree_t *head);
const avl_tree_t *list_find_max(const avl_tree_t *head);

#endif // LINKED_LIST_H__
