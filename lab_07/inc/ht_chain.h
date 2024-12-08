#ifndef HT_CHAIN_H__
#define HT_CHAIN_H__

#include "linked_list.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    list_t **table;
    size_t size;
} ht_chain_t;

int ht_chain_load_file_ex(const char *filename, ht_chain_t **ht);
int ht_chain_load_file(FILE *fp, ht_chain_t **ht);

ht_chain_t *ht_chain_create(size_t size);
void ht_chain_free(ht_chain_t **arr);
int ht_chain_clear(ht_chain_t *arr);

int ht_chain_restruct(ht_chain_t **ht);

int ht_chain_insert(ht_chain_t **arr, const char *key, bool *is_restructured);
int ht_chain_remove(ht_chain_t *arr, const char *key);

int ht_chain_find(const ht_chain_t *arr, const char *key);

int ht_chain_each(const ht_chain_t *arr, list_apply_fn_t func, void *param);

bool ht_chain_check_need_restruct(const ht_chain_t *ht);

extern size_t g_max_collisions;

#endif // HT_CHAIN_H__
