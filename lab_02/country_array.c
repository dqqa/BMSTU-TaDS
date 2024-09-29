#include "country_array.h"
#include "country.h"
#include "errors.h"
#include <assert.h>
#include <string.h>

int ca_input(FILE *fp, country_t *countries, size_t arr_size, size_t *count)
{
    *count = 0;
    int rc = ERR_OK;
    while (rc == ERR_OK)
    {
        country_t c = {0};

        rc = country_input(fp, &c);
        if (rc == ERR_OK)
        {
            if (*count >= arr_size)
                return ERR_OVERFLOW;

            memcpy(countries + *count, &c, sizeof(c));
            (*count)++;
        }
    }

    if (!*count)
        return ERR_EMPTY_FILE;

    return feof(fp) ? ERR_OK : rc;
}

void ca_print(FILE *fp, const country_t *countries, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf("[%zu] ", i);
        country_print(fp, countries + i);
    }
}

int ca_append(FILE *fp, country_t *countries, size_t arr_size, size_t *count)
{
    if (*count >= arr_size)
        return ERR_OVERFLOW;

    country_t c = {0};
    int rc = country_input(fp, &c);
    if (rc != ERR_OK)
        return rc;

    if (*count + 1 >= arr_size)
        return ERR_OVERFLOW;

    memcpy(countries + *count, &c, sizeof(c));
    (*count)++;
    return ERR_OK;
}

int ca_remove(country_t *countries, size_t *count, size_t index)
{
    assert(*count != 0 && "Array size must be > 0");

    if (index >= *count)
        return ERR_PARAMS;

    memmove(countries + index, countries + index + 1, sizeof(*countries) * (*count - index - 1));
    (*count)--;

    return ERR_OK;
}

int ca_sort(country_t *countries);
