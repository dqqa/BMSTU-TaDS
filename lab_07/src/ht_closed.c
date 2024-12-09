#include "ht_closed.h"
#include "errors.h"
#include "hasher.h"
#include "prime_gen.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_HT_SIZE 1

ht_closed_t *ht_closed_create(size_t size)
{
    ht_closed_t *arr = malloc(sizeof(*arr));
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

void ht_closed_free(ht_closed_t **arr)
{
    if (arr == NULL || *arr == NULL)
        return;

    for (size_t i = 0; i < (*arr)->size; i++)
        if ((*arr)->table[i].state == STATE_BUSY)
            free((*arr)->table[i].data);

    free((*arr)->table);
    free(*arr);

    *arr = NULL;
}

int ht_closed_restruct(ht_closed_t **ht, size_t prev_size)
{
    int rc = ERR_OK;
    size_t new_size = calc_next_prime_num(prev_size);
    ht_closed_t *new_ht = ht_closed_create(new_size);

    bool restruct_needed = false;
    for (size_t i = 0; !restruct_needed && i < (*ht)->size; i++)
    {
        if ((*ht)->table[i].state == STATE_EMPTY)
            continue;

        size_t hash = calc_hash_str((*ht)->table[i].data);
        size_t index = hash % new_ht->size;
        size_t new_ind = index;
        size_t collisions = 0;

        while ((new_ht->table[new_ind].state == STATE_REMOVED ||
                (new_ht->table[new_ind].state == STATE_BUSY &&
                 strcmp((*ht)->table[i].data, new_ht->table[new_ind].data) != 0)) &&
               !restruct_needed)
        {
            new_ind = (new_ind + 1) % new_ht->size;
            if (collisions > g_max_collisions)
                restruct_needed = true;

            collisions++;
        }

        if (restruct_needed)
            goto restruct;

        char *key_dup = strdup((*ht)->table[i].data);
        if (key_dup == NULL)
        {
            rc = ERR_ALLOC;
            goto err;
        }

        new_ht->table[new_ind].state = STATE_BUSY;
        new_ht->table[new_ind].data = key_dup;
    }

    err:
    if (rc != ERR_OK)
    {
        ht_closed_free(&new_ht);
        return rc;
    }

    restruct:
    if (restruct_needed)
    {
        ht_closed_free(&new_ht);
        return ht_closed_restruct(ht, new_size);
    }

    ht_closed_free(ht);
    *ht = new_ht;
    return ERR_OK;
}

int ht_closed_insert(ht_closed_t **arr, const char *key, bool *is_restructured)
{
    // TODO!
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    if (is_restructured)
        *is_restructured = false;

    int rc = ERR_OK;
    size_t hash = calc_hash_str(key);

    // Реструктуризация и перехеширование
    while (true)
    {
        bool restruct_needed = false;
        size_t index = hash % (*arr)->size;
        size_t i = index;
        size_t collisions = 0;
        while (((*arr)->table[i].state == STATE_REMOVED ||
                ((*arr)->table[i].state == STATE_BUSY &&
                 strcmp(key, (*arr)->table[i].data) != 0)) &&
               !restruct_needed)
        {
            i = (i + 1) % (*arr)->size;
            if (collisions > g_max_collisions)
                restruct_needed = true;
            collisions++;
        }

        if (!restruct_needed && (*arr)->table[i].state == STATE_BUSY)
            return ERR_REPEAT;

        if (restruct_needed)
        {
            if (is_restructured)
                *is_restructured = true;
            if ((rc = ht_closed_restruct(arr, (*arr)->size)) != ERR_OK)
                goto err;
        }
        else
        {
            char *key_dup = strdup(key);
            if (!key_dup)
            {
                rc = ERR_ALLOC;
                goto err;
            }

            (*arr)->table[i].state = STATE_BUSY;
            (*arr)->table[i].data = key_dup;
            break;
        }
    }

    err:
    return ERR_OK;
}

int ht_closed_find(const ht_closed_t *arr, const char *key, size_t *cmps)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    if (cmps)
        (*cmps) = 1;
    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;
    size_t i = index;

    while (arr->table[i].state == STATE_REMOVED ||
           (arr->table[i].state == STATE_BUSY &&
            strcmp(key, arr->table[i].data) != 0))
    {
        if (cmps)
            (*cmps)++;
        i = (i + 1) % arr->size;
    }

    if (arr->table[i].state == STATE_EMPTY)
        return ERR_NOT_FOUND;

    return ERR_OK;
}

int ht_closed_remove(ht_closed_t *arr, const char *key)
{
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % arr->size;
    size_t i = index;

    while (arr->table[i].state == STATE_REMOVED ||
           (arr->table[i].state == STATE_BUSY &&
            strcmp(key, arr->table[i].data) != 0))
    {
        i = (i + 1) % arr->size;
    }

    if (arr->table[i].state == STATE_EMPTY)
        return ERR_NOT_FOUND;

    arr->table[i].state = STATE_REMOVED;
    free(arr->table[i].data);
    return ERR_OK;
}

int ht_closed_each(const ht_closed_t *arr, ht_closed_apply_fn_t func, void *param)
{
    if (arr == NULL || func == NULL)
        return ERR_PARAM;

    for (size_t i = 0; i < arr->size; i++)
        if (arr->table[i].state == STATE_BUSY)
            func(&arr->table[i], param);

    return ERR_OK;
}

int ht_closed_load_file(FILE *fp, ht_closed_t **ht)
{
    *ht = ht_closed_create(0);
    if (*ht == NULL)
        return ERR_ALLOC;

    int rc = ERR_OK;

    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_IO;
    while (line)
    {
        rc = ht_closed_insert(ht, line, NULL);
        free(line);
        if (rc != ERR_REPEAT && rc != ERR_OK) // TODO
            goto err;

        line = get_str(fp, NULL);
    }

    err:
    if (rc != ERR_REPEAT && rc != ERR_OK)
        ht_closed_free(ht);

    return rc;
}

int ht_closed_load_file_ex(const char *filename, ht_closed_t **ht)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = ht_closed_load_file(fp, ht);

    fclose(fp);
    return rc;
}

float ht_closed_calc_avg_cmp(ht_closed_t *ht)
{
    float res = 0;
    size_t count = 0;
    for (size_t i = 0; i < ht->size; i++)
    {
        if (ht->table[i].state == STATE_EMPTY || ht->table[i].state == STATE_REMOVED)
            continue;

        count++;
        size_t cmps = 0;
        ht_closed_find(ht, ht->table[i].data, &cmps);
        res += cmps;
    }

    return res / count;
}
