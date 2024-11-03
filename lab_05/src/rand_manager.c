#include "rand_manager.h"
#include <stdlib.h>

#define MAX_PERCENT 100

bool rand_with_probability(int prob)
{
    return rand() % MAX_PERCENT < prob;
}

int rand_in_range(int min, int max)
{
    return rand() % (max - min) + min;
}
