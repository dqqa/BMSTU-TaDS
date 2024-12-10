#include "ht_chain.h"
#include "hasher.h"
#include "linked_list.h"
#include "prime_gen.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_HT_SIZE 1

ht_chain_t *ht_chain_create(size_t size)
{
    ht_chain_t *arr = malloc(sizeof(*arr));
    if (arr == NULL)
        return NULL;

    arr->size = size ? size : INITIAL_HT_SIZE;
    arr->table = calloc(arr->size, sizeof(*arr->table));
    if (arr->table == NULL)
    {
        free(arr);
        return NULL;
    }

    return arr;
}

bool ht_chain_check_need_restruct(const ht_chain_t *ht)
{
    assert(ht);
    for (size_t i = 0; i < ht->size; i++)
        if (list_size(ht->table[i]) > g_max_collisions)
            return true;

    return false;
}

int ht_chain_restruct(ht_chain_t **ht)
{
    if (!ht_chain_check_need_restruct(*ht))
        return ERR_NO_NEED_RESTRUCT;

    ht_chain_t *new_ht = NULL;
    size_t cur_size = (*ht)->size;
    int rc = ERR_OK;
    do
    {
        size_t new_size = calc_next_prime_num(cur_size);
        ht_chain_free(&new_ht);
        new_ht = ht_chain_create(new_size);
        if (new_ht == NULL)
        {
            rc = ERR_ALLOC;
            goto err;
        }

        for (size_t i = 0; i < (*ht)->size; i++)
        {
            list_t *cur = (*ht)->table[i];
            while (cur != NULL)
            {
                size_t hash = calc_hash_str(cur->key);
                size_t ind = hash % new_ht->size;
                if ((rc = list_push_back(&new_ht->table[ind], cur->key)) != ERR_OK)
                    goto err;
                cur = cur->next;
            }
        }

        cur_size = new_size;
    } while (ht_chain_check_need_restruct(new_ht));

    err:
    if (rc != ERR_OK)
    {
        ht_chain_free(&new_ht);
    }
    else
    {
        ht_chain_free(ht);
        *ht = new_ht;
    }
    return rc;
}

void ht_chain_free(ht_chain_t **arr)
{
    if (arr == NULL || *arr == NULL)
        return;

    for (size_t i = 0; i < (*arr)->size; i++)
        list_free((*arr)->table + i);

    free((*arr)->table);
    free(*arr);

    *arr = NULL;
}

int ht_chain_insert(ht_chain_t **arr, const char *key, bool *is_restructured)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % (*arr)->size;

    if (list_search_by_key((*arr)->table[index], key, NULL) != NULL)
        return ERR_REPEAT;

    int rc = list_push_back(&(*arr)->table[index], key);
    if (rc != ERR_OK)
        return rc;

    if (is_restructured)
        *is_restructured = false;

    rc = ht_chain_restruct(arr);
    if (is_restructured && rc == ERR_OK)
        *is_restructured = true;

    if (rc == ERR_NO_NEED_RESTRUCT)
        rc = ERR_OK;

    return rc;
}

int ht_chain_find(const ht_chain_t *arr, const char *key, size_t *cmps)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;

    list_t *node = (list_t *)list_search_by_key(arr->table[index], key, cmps);
    if (node == NULL)
        return ERR_NOT_FOUND;

    return ERR_OK;
}

int ht_chain_remove(ht_chain_t *arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;

    return list_remove_by_key(&arr->table[index], key);
}

int ht_chain_clear(ht_chain_t *arr)
{
    if (arr == NULL)
        return ERR_PARAM;

    for (size_t i = 0; i < arr->size; i++)
        list_free(&arr->table[i]);

    return ERR_OK;
}

int ht_chain_each(const ht_chain_t *arr, list_apply_fn_t func, void *param)
{
    if (arr == NULL || func == NULL)
        return ERR_PARAM;

    for (size_t i = 0; i < arr->size; i++)
        if (arr->table[i])
            list_apply(arr->table[i], func, param);

    return ERR_OK;
}

int ht_chain_load_file(FILE *fp, ht_chain_t **ht)
{
    *ht = ht_chain_create(0);
    if (*ht == NULL)
        return ERR_ALLOC;

    int rc = ERR_OK;

    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_IO;

    while (line)
    {
        rc = ht_chain_insert(ht, line, NULL);
        free(line);
        if (rc != ERR_REPEAT && rc != ERR_OK) // TODO
            goto err;

        line = get_str(fp, NULL);
    }

    err:
    if (rc != ERR_REPEAT && rc != ERR_OK)
        ht_chain_free(ht);

    return rc;
}

int ht_chain_load_file_ex(const char *filename, ht_chain_t **ht)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = ht_chain_load_file(fp, ht);

    fclose(fp);
    return rc;
}

static size_t calc_sum_1_to_num(size_t num)
{
    size_t sum = 0;
    for (size_t i = 0; i <= num; i++)
        sum += i;

    return sum;
}

float ht_chain_calc_avg_cmp(ht_chain_t *ht)
{
    float res = 0;
    size_t count = 0;
    for (size_t i = 0; i < ht->size; i++)
    {
        size_t size = list_size(ht->table[i]);
        size_t sum = calc_sum_1_to_num(size);
        res += sum;
        count += size;
    }

    return res / count;
}

void ht_chain_calc_ram_usage(ht_chain_t *ht, size_t *bytes)
{
    (*bytes) = sizeof(ht_chain_t);
    for (size_t i = 0; i < ht->size; i++)
        (*bytes) += list_size(ht->table[i]) * sizeof(**ht->table);
}

void ht_chain_dbg_print(const ht_chain_t *ht)
{
    for (size_t i = 0; i < ht->size; i++)
    {
        printf("%02zu. ", i);
        if (ht->table[i] == NULL)
            printf("NULL");
        else
            list_dbg_print(ht->table[i]);
        printf("\n");
    }
}
