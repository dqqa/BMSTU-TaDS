#ifndef RAND_MANAGER_H__
#define RAND_MANAGER_H__

#include <stdbool.h>

bool rand_with_probability(float prob);
int rand_in_range(int min, int max);

#endif // RAND_MANAGER_H__
