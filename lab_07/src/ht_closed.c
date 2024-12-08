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

    free((*arr)->table);
    free(*arr);

    *arr = NULL;
}

int ht_closed_insert(ht_closed_t **arr, const char *key, bool *is_restructured)
{
    // TODO!
    if (arr == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    size_t hash = calc_hash_str(key);
    size_t index = hash % (*arr)->size;
    size_t i = index;

    bool restruct_needed;
    do
    {
        // Реструктуризация и перехеширование
        restruct_needed = false;
        while (((*arr)->table[i].state == STATE_REMOVED ||
                ((*arr)->table[i].state == STATE_BUSY &&
                 strcmp(key, (*arr)->table[i].data) != 0)) &&
               !restruct_needed)
        {
            i = (i + 1) % (*arr)->size;
            if (index - i > g_max_collisions)
                restruct_needed = true;
        }
        i = index;
    } while (restruct_needed);

    // if (list_search_by_key((*arr)->table[index], key) != NULL)
    //     return ERR_REPEAT;

    // int rc = list_push_back(&(*arr)->table[index], key);
    // if (rc != ERR_OK)
    //     return rc;

    // if (is_restructured)
    //     *is_restructured = false;

    // rc = ht_closed_restruct(arr);
    // if (is_restructured && rc == ERR_OK)
    //     *is_restructured = true;

    // if (rc == ERR_NO_NEED_RESTRUCT)
    //     rc = ERR_OK;
    *is_restructured = false;
    return ERR_OK;
}

int ht_closed_find(const ht_closed_t *arr, const char *key)
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
