#include "bignum.h"
#include "errors.h"
#include "str.h"
#include <assert.h>
#include <inttypes.h>
#include <string.h>

int bignum_parse(bignum_t *num, const char *str)
{
    assert(num && "Invalid pointer");

    size_t new_size = strlen(str);
    size_t i_mantissa = 0, i_exp = 0;
    size_t e_count = 0, point_count = 0;
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

        if (!point_count)
        {
            num->mantissa_whole[num->mantissa_whole_size] = str[i_exp] - '0';
            num->mantissa_whole_size++;
        }
        else
        {
            num->mantissa_frac[num->mantissa_frac_size] = str[i_exp] - '0';
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
    size_t whole_count = 0, frac_count = 0;
    for (size_t i = 0; i < num->mantissa_whole_size; i++)
        if (num->mantissa_whole[i])
            whole_count++;
    for (size_t i = 0; i < num->mantissa_frac_size; i++)
        if (!num->mantissa_frac[i])
            frac_count++;

    memcpy(num->mantissa_frac, num->mantissa_frac + frac_count,
           sizeof(uint8_t) * (num->mantissa_frac_size - frac_count));
    memcpy(num->mantissa_frac + whole_count, num->mantissa_frac, sizeof(uint8_t) * whole_count);
    memcpy(num->mantissa_frac, num->mantissa_whole, sizeof(uint8_t) * whole_count);

    num->mantissa_whole_size = 0;
    num->mantissa_frac_size -= frac_count;
    num->mantissa_frac_size += whole_count;
    num->exponent += whole_count - frac_count;

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

int bignum_multiply(const bignum_t *num1, const bignum_t *num2, bignum_t *result)
{
    memcpy(result, num1, sizeof(*result));
    result->exponent += num2->exponent - 1;
    result->sign *= num2->sign;

    return ERR_OK;
}

// -123e-41
// -0.00123
