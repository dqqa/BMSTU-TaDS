#ifndef BIGNUM_H_
#define BIGNUM_H_

#include "constants.h"
#include <inttypes.h>

typedef struct
{
    int8_t sign;
    uint16_t mantissa_whole_size;
    uint16_t mantissa_frac_size;
    uint8_t mantissa_whole[MAX_MANTISSA_SIZE];
    uint8_t mantissa_frac[MAX_MANTISSA_SIZE];
    int16_t exponent;
} bignum_t;

int bignum_parse(bignum_t *num, const char *str);
int bignum_print(const bignum_t *num);
int bignum_validate(const bignum_t *num);
int bignum_multiply(const bignum_t *num1, const bignum_t *num2, bignum_t *result);
int bignum_normalize(bignum_t *num);

#endif // BIGNUM_H_
