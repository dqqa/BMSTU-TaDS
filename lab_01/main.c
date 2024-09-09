#include "bignum.h"
#include "constants.h"
#include "errors.h"
#include "str.h"
#include <stdio.h>

int main(void)
{
    char num1_buf[MAX_INPUT_BUF_SIZE];
    int rc = ERR_OK;
    (void)rc;
    fgets(num1_buf, sizeof(num1_buf), stdin);
    size_t newlen = str_strip(num1_buf);
    printf("newlen: %zu, str: [%s]\n", newlen, num1_buf);

    // if ((rc = str_input(stdin, num1_buf, sizeof(num1_buf))) != ERR_OK)
    // {
    //     print_err(rc);
    //     return rc;
    // }

    // bignum_t bignum_1, bignum_2, result;
    // if ((rc = bignum_init(&bignum_1, num1_buf)) != ERR_OK)
    // {
    //     print_err(rc);
    //     return rc;
    // }

    return ERR_OK;
}
