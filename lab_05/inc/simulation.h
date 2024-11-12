#ifndef SIMULATION_H__
#define SIMULATION_H__

#include "common.h"
#include <stddef.h>

extern int T1_LOWER;
extern int T1_UPPER;

extern int T2_LOWER;
extern int T2_UPPER;

extern float P;

int simulate_first_n(size_t n, queue_base_t *queue, float *time);

#endif // SIMULATION_H__
