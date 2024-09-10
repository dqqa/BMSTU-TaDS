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
    size_t i_mantissa = 0, i_exp = 0;
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
    size_t whole_count = num->mantissa_whole_size, frac_leading_count = 0, frac_ending_count = 0;

    for (size_t i = 0; i < num->mantissa_frac_size; i++)
    {
        if (!num->mantissa_frac[i])
            frac_leading_count++;
        else
            break;
    }

    memcpy(num->mantissa_frac + whole_count, num->mantissa_frac, sizeof(uint8_t) * num->mantissa_frac_size);
    memcpy(num->mantissa_frac, num->mantissa_whole, sizeof(uint8_t) * whole_count);

    num->mantissa_frac_size += whole_count;

    for (long i = num->mantissa_frac_size - 1; i >= 0 && !num->mantissa_frac[i]; i--)
        frac_ending_count++;

    num->mantissa_whole_size = 0;
    num->mantissa_frac_size -= frac_ending_count;
    num->exponent += whole_count;

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

void shift_array_right(uint8_t *arr, size_t size, size_t amount)
{
    memmove(arr + amount, arr, sizeof(*arr) * size);
    memset(arr, 0, sizeof(*arr) * amount);
}

int bignum_multiply(const bignum_t *num1, const bignum_t *num2, bignum_t *result)
{
    int rc = ERR_OK;
    memcpy(result, num2, sizeof(*result));

    memset(result->mantissa_frac, 0, sizeof(result->mantissa_frac));
    memset(result->mantissa_whole, 0, sizeof(result->mantissa_whole));

    result->exponent += num1->exponent - 1;
    result->sign *= num1->sign;

    size_t max_shift = 0;
    int remainder = 0;
    for (long i = num2->mantissa_frac_size - 1; i >= 0; i--)
    {
        remainder = 0;
        for (long j = num1->mantissa_frac_size - 1; j >= 0; j--)
        {
            /////////////////////////////////////////////////////////////////////////////////
            //////////// Определить когда необходимо делать сдвиг и на сколько //////////////
            /////////////////////////////////////////////////////////////////////////////////
            
            if ((i - j) < 0 && ABS(i - j) > max_shift)
            {
                max_shift = ABS(i - j);
                shift_array_right(result->mantissa_frac, result->mantissa_frac_size, ABS(i - j));
                result->mantissa_frac_size+=ABS(i-j);
                // result->exponent++; // (?)
            }

            result->mantissa_frac[i - (num1->mantissa_frac_size - j - 1) + max_shift] += num2->mantissa_frac[i] * num1->mantissa_frac[j] + remainder;
            remainder = result->mantissa_frac[i - (num1->mantissa_frac_size - j - 1) + max_shift] / 10;

            result->mantissa_frac[i - (num1->mantissa_frac_size - j - 1) + max_shift] %= 10;
        }

        if (remainder)
        {
            shift_array_right(result->mantissa_frac, result->mantissa_frac_size, 1);

            // TODO: Check if remainder > 10
            result->mantissa_frac[0] = remainder;
            result->mantissa_frac_size++;
            if (i != 0)
                result->exponent++; // (?)
            max_shift++;
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
*/
