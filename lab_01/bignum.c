#include "bignum.h"
#include <inttypes.h>

int bignum_parse(bignum_t *num, const char *str);
int bignum_print(const bignum_t *num);
int bignum_validate(const bignum_t *num);
int bignum_divide(const bignum_t *numerator, const bignum_t *denominator, bignum_t *result);
