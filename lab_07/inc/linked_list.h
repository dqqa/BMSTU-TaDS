#ifndef LINKED_LIST_H__
#define LINKED_LIST_H__

#include "errors.h"
#include "node.h"

typedef void (*list_apply_fn_t)(const char *key, int *num, void *param);

void list_free(node_t **head);

int push_back(node_t **head, const char *key, int val);

void list_apply(node_t *head, list_apply_fn_t func, void *arg);

const node_t *list_search_by_key(const node_t *head, const char *key);

int list_remove_by_key(node_t **head, const char *key);

const node_t *list_find_min(const node_t *head);
const node_t *list_find_max(const node_t *head);

#endif // LINKED_LIST_H__
