#ifndef SORT_H_
#define SORT_H_

#include <stdlib.h>

typedef int (*cmp_fun_t)(const void *, const void *);

void sort_bubble(void *base, size_t arr_size, size_t el_size, cmp_fun_t comp);

#endif // SORT_H_
