#include "bignum.h"
#include "errors.h"
#include "str.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

int check_exp(int exp)
{
    if (exp > EXPONENT_MAX)
        return ERR_OVERFLOW;
    else if (exp < EXPONENT_MIN)
        return ERR_UNDERFLOW;
    return ERR_OK;
}

int bignum_parse_real(bignum_t *num, const char *str)
{
    assert(num && "Invalid pointer");

    size_t new_size = strlen(str);
    size_t i_exp = 0;
    size_t e_count = 0, point_count = 0;

    if (!new_size)
        return ERR_IO;

    if (strchr(SIGNS, str[0]))
    {
        if (str[0] == SIGN_MINUS)
            num->sign = -1;
        else
            num->sign = 1;

        str++;
        new_size--;
    }
    else
        num->sign = 1;

    num->mantissa_frac_size = 0;
    num->exponent = 0;

    for (i_exp = 0; i_exp < new_size; i_exp++)
    {
        if (str[i_exp] == SYMBOL_DECIMAL_DOT)
        {
            if (!point_count)
            {
                point_count++;
                continue;
            }
            else
                return ERR_NUMBER;
        }

        if (strchr(SYMBOLS_EXPONENT, str[i_exp]))
        { //  str[i_exp] == 'e' || str[i_exp] == 'E'
            if (e_count || !num->mantissa_frac_size)
                return ERR_NUMBER;

            e_count++;
            break;
        }

        if (str[i_exp] > DIGIT_MAX || str[i_exp] < DIGIT_MIN)
            return ERR_NUMBER;
        if (num->mantissa_frac_size > MAX_MANTISSA_SIZE / 2 - 1)
            return ERR_OVERFLOW;

        uint8_t digit = str[i_exp] - DIGIT_MIN;
        if (!point_count)
        {
            num->mantissa_frac[num->mantissa_frac_size] = digit;
            num->exponent++;
        }
        else
            num->mantissa_frac[num->mantissa_frac_size] = digit;
        num->mantissa_frac_size++;
    }

    int32_t new_exp;
    if (e_count)
    {
        int pos;
        if (sscanf(str + i_exp + 1, "%" SCNd32 "%n", &new_exp, &pos) != 1)
            return ERR_NUMBER;
        if (pos + i_exp + 1 != new_size)
            return ERR_NUMBER;

        num->exponent += new_exp;
        int rc;
        if ((rc = check_exp(num->exponent) != ERR_OK))
            return rc;
    }

    bignum_normalize(num);

    return ERR_OK;
}

int bignum_parse_int(bignum_t *num, const char *str)
{
    assert(num && "Invalid pointer");

    size_t new_size = strlen(str);
    size_t i_exp = 0;

    if (!new_size)
        return ERR_IO;

    if (strchr(SIGNS, str[0]))
    {
        if (str[0] == SIGN_MINUS)
            num->sign = -1;
        else
            num->sign = 1;

        str++;
        new_size--;
    }
    else
        num->sign = 1;

    num->mantissa_frac_size = 0;
    num->exponent = 0;

    for (i_exp = 0; i_exp < new_size; i_exp++)
    {
        if (str[i_exp] > DIGIT_MAX || str[i_exp] < DIGIT_MIN)
            return ERR_NUMBER;
        if (num->mantissa_frac_size > MAX_MANTISSA_SIZE / 2)
            return ERR_OVERFLOW;

        uint8_t digit = str[i_exp] - DIGIT_MIN;

        num->mantissa_frac[num->mantissa_frac_size] = digit;
        num->exponent++;
        num->mantissa_frac_size++;
    }
    // MAYBE? if (num->mantissa_frac_size > 30)

    if (i_exp != new_size)
        return ERR_NUMBER;
    bignum_normalize(num);

    return ERR_OK;
}

int bignum_normalize(bignum_t *num)
{
    size_t frac_ending_count = 0;

    for (long i = num->mantissa_frac_size - 1; i >= 0 && !num->mantissa_frac[i]; i--)
        frac_ending_count++;

    num->mantissa_frac_size -= frac_ending_count;

    for (size_t i = 0; i < num->mantissa_frac_size && !num->mantissa_frac[i]; i++)
    {
        memmove(num->mantissa_frac, num->mantissa_frac + 1, num->mantissa_frac_size - 1);
        num->mantissa_frac_size--;
        num->exponent--;
        i--;
    }

    // Shouldn't be an error
    // if (num->mantissa_frac_size > MAX_MANTISSA_SIZE / 2)
    //     return ERR_OVERFLOW;

    if (num->mantissa_frac_size > MAX_MANTISSA_SIZE / 2)
    {
        // Алгоритм округления мантиссы до MAX_MANTISSA_SIZE / 2 знаков.
        int remainder = 0;
        if (num->mantissa_frac[MAX_MANTISSA_SIZE / 2] >= 5)
        {
            remainder = 1;
            for (long i = MAX_MANTISSA_SIZE / 2 - 1; i >= 0; i--)
            {
                num->mantissa_frac[i] += remainder;
                remainder = num->mantissa_frac[i] / 10;
                num->mantissa_frac[i] %= 10;

                if (!remainder)
                    break;
            }
        }
        if (remainder)
        {
            num->mantissa_frac_size = 1;
            num->mantissa_frac[0] = remainder;
            num->exponent++;
        }
        else
            num->mantissa_frac_size = MAX_MANTISSA_SIZE / 2;
    }

    frac_ending_count = 0;
    for (long i = num->mantissa_frac_size - 1; i >= 0 && !num->mantissa_frac[i]; i--)
        frac_ending_count++;

    num->mantissa_frac_size -= frac_ending_count;

    int rc;
    if ((rc = check_exp(num->exponent)) != ERR_OK)
        return rc;

    if (num->mantissa_frac_size == 0)
    {
        num->mantissa_frac_size++;
        num->mantissa_frac[0] = 0;
        num->exponent = 0;
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

    result->exponent += left->exponent;
    result->sign *= left->sign;
    if (result->exponent > EXPONENT_MAX)
        return ERR_OVERFLOW;
    if (result->exponent < EXPONENT_MIN)
        return ERR_UNDERFLOW;

    size_t max_shift = left->mantissa_frac_size;
    int remainder = 0;

    // This shouldn't be an error
    // if (max_shift + right->mantissa_frac_size > MAX_MANTISSA_SIZE)
    //     return ERR_OVERFLOW;

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

int bignum_divide(const bignum_t *num1, const bignum_t *num2, bignum_t *result);

// int bignum_add(const bignum_t *num1, const bignum_t *num2, bignum_t *result)
// {
//     memcpy(result, num1, sizeof(result));
//     int32_t new_exp = MAX(num1->exponent, num2->exponent);
//     if (num1->exponent >= num2->exponent)
//     {
//         if ((num1->exponent - num2->exponent) + num1->mantissa_frac_size + num2->mantissa_frac_size > MAX_MANTISSA_SIZE / 2)
//             return ERR_OVERFLOW;

//         new_exp = num1->exponent;
//         shift_array_right(num2->mantissa_frac, &num2->mantissa_frac_size, num1->exponent - num2->exponent);
//     }
//     else
//     {
//         if ((num1->exponent - num2->exponent) + num1->mantissa_frac_size + num2->mantissa_frac_size > MAX_MANTISSA_SIZE / 2)
//             return ERR_OVERFLOW;

//         new_exp = num1->exponent;
//         shift_array_right(num2->mantissa_frac, &num2->mantissa_frac_size, num1->exponent - num2->exponent);
//     }
//     result->exponent = new_exp;
//     // shift_array_right()
//     for (long i = num2->mantissa_frac_size - 1; i >= 0; i--)
//     {
//         long modifier = result->exponent - num2->exponent;
//     }
// }

int bignum_sub(const bignum_t *num1, const bignum_t *num2, bignum_t *result);

/*
-123e-41
-0.00123
=> 0.15129e-41

1.12e123
10.5e15
=> 0.1176e140

0.1e1
0.2e2
=> 0.2e2

1000
1
=>0.1e4

100.01
0.1
=> 0.10001e2

-131.e156
831.4
=> -0.1089134e162

99
99
=> 0.9801e4

999
999
=> 0.998001e6

999.99
999.99
=> 0.9999800001e6
*/
