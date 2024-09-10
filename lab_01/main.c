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

    if ((rc = str_input(stdin, num1_buf, sizeof(num1_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }
    if ((rc = str_input(stdin, num2_buf, sizeof(num1_buf))) != ERR_OK)
    {
        print_err(rc);
        return rc;
    }
    str_strip(num1_buf);
    bignum_t bignum_1, bignum_2, result;
    rc = bignum_parse(&bignum_1, num1_buf);
    printf("RC: %d\n", rc);

    rc = bignum_parse(&bignum_2, num2_buf);
    printf("RC: %d\n", rc);
    
    bignum_multiply(&bignum_1, &bignum_2, &result);

    bignum_print(&bignum_1);
    bignum_print(&bignum_2);

    bignum_print(&result);

    return ERR_OK;
}
