#ifndef COUNTRY_ARRAY_H_
#define COUNTRY_ARRAY_H_

#include <stdio.h>
#include "country.h"

int ca_input(FILE *fp, country_t *countries, size_t arr_size, size_t *count);
int ca_append(FILE *fp, country_t *countries, size_t arr_size, size_t *count);
void ca_print(FILE *fp, const country_t *countries, size_t count);
int ca_sort(country_t *countries);

#endif // COUNTRY_ARRAY_H_
