#include "rand_manager.h"
#include <stdlib.h>

#define MAX_PERCENT 100

/**
 * @brief Генерация случайного значения (Да/ Нет) в зависимости от вероятности
 * 
 * @param prob вероятность (от 0.0f до 1.0f)
 * @return true Да \
 * @return false Нет
 */
bool rand_with_probability(float prob)
{
    return (float)rand() / RAND_MAX < prob;
}

int rand_in_range_int(int min, int max)
{
    return rand() % (max - min) + min;
}

float rand_in_range_float(float min, float max)
{
    return (float)rand() / RAND_MAX * (max - min) + min;
}
