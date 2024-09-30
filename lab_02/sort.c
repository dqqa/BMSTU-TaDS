#include "sort.h"

static void swap(void *a, void *b, size_t el_size)
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
