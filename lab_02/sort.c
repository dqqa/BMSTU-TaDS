#include "sort.h"
#include "errors.h"
#include <string.h>

static inline void swap(void *a, void *b, size_t el_size)
{
    for (size_t i = 0; i < el_size; i++)
    {
        char tmp = *((char *)a + i);
        *((char *)a + i) = *((char *)b + i);
        *((char *)b + i) = tmp;
    }
}

void sort_bubble(void *base, size_t arr_size, size_t el_size, cmp_fun_t comp)
{
    for (size_t i = arr_size; i > 1; i--)
    {
        for (size_t j = 1; j < i; j++)
        {
            void *l = ((char *)base) + el_size * (j - 1);
            void *r = ((char *)base) + el_size * j;

            if (comp(l, r) > 0)
                swap(l, r, el_size);
        }
    }
}

int key_record_int_cmp(const void *l, const void *r)
{
    const key_record_t *lp = l, *rp = r;
    return lp->data > rp->data;
}

int key_table_create(const country_t *countries, size_t countries_count, key_record_t *keytable, size_t keytable_size, size_t *keytable_count, field_t field)
{
    if (countries_count > keytable_size)
        return ERR_OVERFLOW;

    *keytable_count = 0;

    for (size_t i = 0; i < countries_count; i++)
    {
        (*keytable_count)++;
        keytable[i].table_index = i;

        switch (field)
        {
            case FIELD_TRAVEL_TIME:
                keytable[i].data = countries[i].travel_time;
                break;
            case FIELD_MIN_VACATION_COST:
                keytable[i].data = countries[i].min_vacation_cost;
                break;
            default:
                break;
        }
    }

    return ERR_OK;
}

void key_table_print(key_record_t *keytable, size_t keytable_size, field_t field)
{
    for (size_t i = 0; i < keytable_size; i++)
    {
        printf("[%zu] ", keytable[i].table_index);
        switch (field)
        {
            case FIELD_TRAVEL_TIME:
            case FIELD_MIN_VACATION_COST:
                printf("%" PRIu32, keytable[i].data);
                break;
            default:
                break;
        }
        printf("\n");
    }
}
