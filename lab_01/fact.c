#include "bignum.h"
#include "constants.h"
#include "errors.h"
#include "str.h"
#include <stdio.h>

void fact(void)
{
    int iter_count, rc = ERR_OK;
    bignum_t num = {0}, m = {0};
    bignum_parse(&num, "1");
    bignum_parse(&m, "2");

    printf("Input iter count: ");
    if (scanf("%d", &iter_count) != 1)
    {
        print_err(ERR_IO);
        return ERR_IO;
    }
    for (int i = 0; i < iter_count; i++)
    {
        printf("2^%d = ", i);
        bignum_print(&num);

        bignum_t new = {0};
        if ((rc = bignum_multiply(&num, &m, &new)) != ERR_OK)
        {
            print_err(rc);
            return;
        }
        num = new;
    }
}

// 633825300114114700748351602688
// 696898287454081973172991196020261297061888
// 766247770432944429179173513575154591809369561091801088
// 1018517988167243043134222844204689080525734196832968125318070224677190649881668353091698688
