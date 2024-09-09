#ifndef BIGNUM_H_
#define BIGNUM_H_

#include "constants.h"
#include <inttypes.h>

typedef struct
{
    int8_t sign;
    uint8_t mantissa[MAX_MANTISSA_SIZE];
    int16_t exponent;
} bignum_t;

int bignum_parse(bignum_t *num, const char *str);
int bignum_print(const bignum_t *num);
int bignum_validate(const bignum_t *num);
int bignum_divide(const bignum_t *numerator, const bignum_t *denominator, bignum_t *result);

#endif // BIGNUM_H_
