#ifndef LINKED_LIST_H__
#define LINKED_LIST_H__

#include "errors.h"
#include "common.h"

typedef struct __linked_list_node list_t;

struct __linked_list_node
{
    data_t key;
    list_t *next;
};

list_t *list_node_create(void);

typedef void (*list_apply_fn_t)(list_t *node, void *param);

void list_free(list_t **head);

int list_push_back(list_t **head, const char *key);

void list_apply(list_t *head, list_apply_fn_t func, void *arg);

const list_t *list_search_by_key(const list_t *head, const char *key);

int list_remove_by_key(list_t **head, const char *key);

const list_t *list_find_min(const list_t *head);
const list_t *list_find_max(const list_t *head);

#endif // LINKED_LIST_H__
