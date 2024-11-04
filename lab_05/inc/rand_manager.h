#ifndef RAND_MANAGER_H__
#define RAND_MANAGER_H__

#include <stdbool.h>

bool rand_with_probability(float prob);
int rand_in_range_int(int min, int max);
float rand_in_range_float(float min, float max);

#endif // RAND_MANAGER_H__
