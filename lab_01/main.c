#include "bignum.h"
#include "constants.h"
#include "errors.h"
#include "str.h"
#include <stdio.h>

int main(void)
{
    char num1_buf[MAX_INPUT_BUF_SIZE];
    char num2_buf[MAX_INPUT_BUF_SIZE];
    int rc = ERR_OK;
    printf("Input first big number: ");
    if ((rc = str_input(stdin, num1_buf, sizeof(num1_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    printf("Input second big number: ");
    if ((rc = str_input(stdin, num2_buf, sizeof(num2_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    size_t new_size1 = str_strip(num1_buf);
    size_t new_size2 = str_strip(num2_buf);

    bignum_t bignum_1, bignum_2, result;

    if ((rc = bignum_parse(&bignum_1, num1_buf)) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    printf("RC: %d\n", rc);
    printf("Num 1 parsed: ");
    bignum_print(&bignum_1);

    if ((rc = bignum_parse(&bignum_2, num2_buf)) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }

    printf("RC: %d\n", rc);
    printf("Num 2 parsed: ");
    bignum_print(&bignum_2);

    bignum_multiply(&bignum_1, &bignum_2, &result);

    printf("Result: ");
    bignum_print(&result);

    return ERR_OK;
}
