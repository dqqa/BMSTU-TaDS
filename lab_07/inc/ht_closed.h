#ifndef HT_CLOSED_H__
#define HT_CLOSED_H__

#include "common.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum
{
    STATE_EMPTY,
    STATE_REMOVED,
    STATE_BUSY
} state_t;

struct __ht_closed_item
{
    state_t state;
    data_t data;
};

typedef struct
{
    struct __ht_closed_item *table;
    size_t size;
} ht_closed_t;

typedef void (*ht_closed_apply_fn_t)(struct __ht_closed_item *item, void *arg);

int ht_closed_load_file_ex(const char *filename, ht_closed_t **ht);
int ht_closed_load_file(FILE *fp, ht_closed_t **ht);

ht_closed_t *ht_closed_create(size_t size);
void ht_closed_free(ht_closed_t **arr);

int ht_closed_insert(ht_closed_t **arr, const char *key, bool *is_restructured);
int ht_closed_remove(ht_closed_t *arr, const char *key);

int ht_closed_find(const ht_closed_t *arr, const char *key, size_t *cmps);

int ht_closed_each(const ht_closed_t *arr, ht_closed_apply_fn_t func, void *param);

float ht_closed_calc_avg_cmp(ht_closed_t *ht);

extern size_t g_max_collisions;

#endif // HT_CLOSED_H__
