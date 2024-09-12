#include "bignum.h"
#include "errors.h"
#include "str.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

int bignum_parse(bignum_t *num, const char *str)
{
    assert(num && "Invalid pointer");

    size_t new_size = strlen(str);
    size_t i_exp = 0;
    size_t e_count = 0, point_count = 0;
    bool starting_with_zero = false;
    if (!new_size)
        return ERR_IO;

    if (str[0] == '-')
    {
        num->sign = -1;
        str++;
        new_size--;
    }
    else if (str[0] == '+')
    {
        num->sign = 1;
        str++;
        new_size--;
    }
    else
        num->sign = 1;

    num->mantissa_frac_size = 0;
    num->mantissa_whole_size = 0;
    num->exponent = 0;
    for (i_exp = 0; i_exp < new_size; i_exp++)
    {
        if (str[i_exp] == '.')
        {
            if (!point_count)
            {
                point_count++;
                continue;
            }
            else
                return ERR_NUMBER;
        }

        if (str[i_exp] == 'e' || str[i_exp] == 'E')
        {
            if (e_count)
                return ERR_NUMBER;

            e_count++;
            break;
        }

        if (str[i_exp] > '9' || str[i_exp] < '0')
            return ERR_NUMBER;

        uint8_t digit = str[i_exp] - '0';
        if (!point_count)
        {
            if (!i_exp && digit == 0)
                starting_with_zero = true;

            num->mantissa_whole[num->mantissa_whole_size] = digit;
            if (!starting_with_zero)
                num->mantissa_whole_size++;
        }
        else
        {
            num->mantissa_frac[num->mantissa_frac_size] = digit;
            num->mantissa_frac_size++;
        }
    }

    if (e_count)
        if (sscanf(e_count ? str + i_exp + 1 : str + i_exp, "%" SCNd16, &num->exponent) != 1)
            return ERR_IO;

    bignum_normalize(num);

    return ERR_OK;
}

int bignum_normalize(bignum_t *num)
{
    size_t whole_count = num->mantissa_whole_size, frac_ending_count = 0;

    memcpy(num->mantissa_frac + whole_count, num->mantissa_frac, sizeof(uint8_t) * num->mantissa_frac_size);
    memcpy(num->mantissa_frac, num->mantissa_whole, sizeof(uint8_t) * whole_count);

    num->mantissa_frac_size += whole_count;

    for (long i = num->mantissa_frac_size - 1; i >= 0 && !num->mantissa_frac[i]; i--)
        frac_ending_count++;

    num->mantissa_whole_size = 0;
    num->mantissa_frac_size -= frac_ending_count;
    num->exponent += whole_count;

    for (size_t i = 0; i < num->mantissa_frac_size && !num->mantissa_frac[i]; i++)
    {
        memmove(num->mantissa_frac, num->mantissa_frac + 1, num->mantissa_frac_size - 1);
        num->mantissa_frac_size--;
        num->exponent--;
        i--;
    }
    return ERR_OK;
}

void bignum_print(const bignum_t *num)
{
    if (num->sign < 0)
        printf("-");
    printf("0.");
    for (size_t i = 0; i < num->mantissa_frac_size; i++)
        printf("%" PRIu8, num->mantissa_frac[i]);

    printf("e%" PRId16 "\n", num->exponent);
}

int bignum_validate(const bignum_t *num);

static void shift_array_right(uint8_t *arr, uint16_t *size, size_t amount)
{
    memmove(arr + amount, arr, sizeof(*arr) * *size);
    memset(arr, 0, sizeof(*arr) * amount);
    *size += amount;
}

int bignum_multiply(const bignum_t *left, const bignum_t *right, bignum_t *result)
{
    int rc = ERR_OK;
    memcpy(result, right, sizeof(*result));

    memset(result->mantissa_frac, 0, sizeof(result->mantissa_frac));
    memset(result->mantissa_whole, 0, sizeof(result->mantissa_whole));

    result->exponent += left->exponent;
    result->sign *= left->sign;

    size_t max_shift = left->mantissa_frac_size;
    int remainder = 0;
   
    if (max_shift + right->mantissa_frac_size > MAX_MANTISSA_SIZE)
        return ERR_OVERFLOW;

    /*
        Сдвигаем мантиссу result на right->mantissa_frac_size + left->mantissa_frac_size
        (максиммальное возможное число имеет столько разрядов при перемножении)
    */

    shift_array_right(result->mantissa_frac, &result->mantissa_frac_size, max_shift);

    for (long i = right->mantissa_frac_size - 1; i >= 0; i--)
    {
        remainder = 0;
        for (long j = left->mantissa_frac_size - 1; j >= 0; j--)
        {
            size_t new_index = i - (left->mantissa_frac_size - j - 1) + max_shift;
            int digit_mult = right->mantissa_frac[i] * left->mantissa_frac[j];

            result->mantissa_frac[new_index] += digit_mult + remainder;
            remainder = result->mantissa_frac[new_index] / 10;
            result->mantissa_frac[new_index] %= 10;
        }

        if (remainder)
        {
            long ind = i - left->mantissa_frac_size + max_shift;
            while (remainder)
            {
                result->mantissa_frac[ind] = remainder % 10;
                remainder /= 10;
                ind--;
            }
        }
    }

    if ((rc = bignum_normalize(result)) != ERR_OK)
        return rc;

    return ERR_OK;
}

// -123e-41
// -0.00123

/*
1.12e123
10.5e15

0.1e1
0.2e2

1000
1

100.01
0.1

-131.e156
831.4

99
99
=> 0.9801e4

999
999
=> 0.998001e6
*/
