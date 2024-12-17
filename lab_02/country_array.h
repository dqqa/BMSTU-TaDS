#ifndef COUNTRY_ARRAY_H_
#define COUNTRY_ARRAY_H_

#include "country.h"
#include "sort.h"
#include <stdio.h>

typedef int (*filter_fun_t)(const country_t *c, const country_t *params);

int ca_input(FILE *fp, country_t *countries, size_t arr_size, size_t *count);
int ca_append(FILE *fp, country_t *countries, size_t arr_size, size_t *count);
int ca_remove(country_t *countries, size_t *count, size_t index);
void ca_print(FILE *fp, const country_t *countries, size_t count);
void ca_print_key(FILE *fp, const country_t *countries, size_t count, const key_record_t *keytable);
void ca_filter(country_t *countries, size_t *count, filter_fun_t filter, const country_t *params);

#endif // COUNTRY_ARRAY_H_
