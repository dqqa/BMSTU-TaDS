#include "bignum.h"
#include "constants.h"
#include "errors.h"
#include "str.h"
#include <stdio.h>
#include <string.h>

#define DO_TEST 0
#define DO_DEBUG_PRINT 0

// Посчитать факториал от заданного числа
void fact(void);

void test(void)
{
    bignum_t num = {0};
    memset(num.mantissa_frac, 9, 41);
    num.mantissa_frac[10] = 8;
    // memset(num.mantissa_frac+30, 8, 11);
    num.mantissa_frac_size = 41;
    num.exponent = 45;

    bignum_normalize(&num);
    bignum_print(&num);
}

void draw_ruler(size_t init_space, size_t length)
{
    for (size_t i = 0; i < init_space - 1; i++)
        printf(" ");
    printf(">");

    for (size_t i = 1; i <= length; i++)
    {
        if (i % 10 == 0)
            printf("%zu", i / 10);
        else
            printf("-");
    }
    printf("-->, length");
    printf("\n");
}

int main(void)
{
#if !DO_TEST
    char num1_buf[MAX_INPUT_BUF_SIZE];
    char num2_buf[MAX_INPUT_BUF_SIZE];
    int rc = ERR_OK;
    printf("Программа для умножения действительного числа на целое.\n\n");
    draw_ruler(23, 80);

    bignum_t bignum_1, bignum_2, result;

    printf("Input real big number: ");
    if ((rc = str_input(stdin, num1_buf, sizeof(num1_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    /*size_t new_size1 = */ str_strip(num1_buf);

    if ((rc = bignum_parse_real(&bignum_1, num1_buf)) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }
#if DO_DEBUG_PRINT
    printf("RC: %d\n", rc);
    printf("Num 1 parsed: ");
    bignum_print(&bignum_1);
#endif
    printf("Input int  big number: ");
    if ((rc = str_input(stdin, num2_buf, sizeof(num2_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    /*size_t new_size2 = */ str_strip(num2_buf);

    if ((rc = bignum_parse_int(&bignum_2, num2_buf)) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }
#if DO_DEBUG_PRINT
    printf("RC: %d\n", rc);
    printf("Num 2 parsed: ");
    bignum_print(&bignum_2);
#endif
    if ((rc = bignum_multiply(&bignum_1, &bignum_2, &result)) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    printf("Result: ");
    bignum_print(&result);
#else
    // fact();
    test();
#endif
    return ERR_OK;
}
