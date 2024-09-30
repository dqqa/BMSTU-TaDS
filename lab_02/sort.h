#ifndef SORT_H_
#define SORT_H_

#include "country.h"
#include <stdlib.h>

typedef struct
{
    size_t table_index;
    const void *data;
} key_record_t;

typedef int (*cmp_fun_t)(const void *, const void *);

int key_table_create(const country_t *countries, size_t countries_count, key_record_t *keytable, size_t keytable_size, size_t *keytable_count, field_t field);
void key_table_print(key_record_t *keytable, size_t keytable_size, field_t field);

void sort_bubble(void *base, size_t arr_size, size_t el_size, cmp_fun_t comp);

int key_record_int_cmp(const void *l, const void *r);
int key_record_str_cmp(const void *l, const void *r);

#endif // SORT_H_
