#ifndef HT_CHAIN_H__
#define HT_CHAIN_H__

#include "linked_list.h"
#include <stddef.h>

typedef struct
{
    list_t **table;
    size_t size;
} ht_chain_t;

ht_chain_t *ht_chain_create(void);
void ht_chain_destroy(ht_chain_t **arr);
int ht_chain_clear(ht_chain_t *arr);

int ht_chain_insert(ht_chain_t *arr, const char *key, int num);
int ht_chain_remove(ht_chain_t *arr, const char *key);

int ht_chain_find(const ht_chain_t *arr, const char *key);

int ht_chain_each(const ht_chain_t *arr, list_apply_fn_t func, void *param);

#endif // HT_CHAIN_H__
