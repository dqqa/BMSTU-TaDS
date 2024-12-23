#ifndef BIGNUM_H_
#define BIGNUM_H_

#include "constants.h"
#include <inttypes.h>

#define DIGIT_MIN '0'
#define DIGIT_MAX '9'

#define EXPONENT_MIN -99999
#define EXPONENT_MAX 99999
#define SYMBOLS_EXPONENT "eE"

#define SIGN_MINUS '-'
#define SIGN_PLUS '+'
#define SIGNS "+-"

#define SYMBOL_DECIMAL_DOT '.'

typedef struct
{
    int8_t sign;
    uint16_t mantissa_frac_size;
    uint8_t mantissa_frac[MAX_MANTISSA_SIZE];
    int32_t exponent;
} bignum_t;

int bignum_parse_real(bignum_t *num, const char *str);
int bignum_parse_int(bignum_t *num, const char *str);
void bignum_print(const bignum_t *num);
int bignum_multiply(const bignum_t *num1, const bignum_t *num2, bignum_t *result);
int bignum_divide(const bignum_t *num1, const bignum_t *num2, bignum_t *result);
int bignum_add(const bignum_t *num1, const bignum_t *num2, bignum_t *result);
int bignum_sub(const bignum_t *num1, const bignum_t *num2, bignum_t *result);
int bignum_normalize(bignum_t *num);

/*
TODO:
    - bignum_cmp
    - bignum_divide
    - bignum_add
*/

#endif // BIGNUM_H_
