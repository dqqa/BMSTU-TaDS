#include "bignum.h"
#include "constants.h"
#include "errors.h"
#include "str.h"
#include <stdio.h>
#include <time.h>

#define MS_MULTIPLIER 1000
#define US_MULTIPLIER 1000000

void fact(void)
{
    int iter_count, rc = ERR_OK;
    clock_t start, end;
    bignum_t num = {0}, m = {0};
    bignum_parse_real(&num, "1");
    bignum_parse_real(&m, "256");

    printf("Input iter count: ");
    if (scanf("%d", &iter_count) != 1)
    {
        print_err(ERR_IO);
        return;
    }
    start = clock();
    int i;
    for (i = 0; i < iter_count; i++)
    {
        // printf("256^%d = ", i);
        // bignum_print(&num);

        bignum_t new = {0};
        if ((rc = bignum_multiply(&num, &m, &new)) != ERR_OK)
        {
            print_err(rc);
            break;
        }
        num = new;
    }
    end = clock();
    printf("Latest number: 256^%d = ", i);
    bignum_print(&num);
    printf("Total elapsed: %fms\n", (float)(end - start) / CLOCKS_PER_SEC * MS_MULTIPLIER);
    printf("Avg iteration: %fus\n", (float)(end - start) / CLOCKS_PER_SEC / i * US_MULTIPLIER);
}

// 633825300114114700748351602688
// 696898287454081973172991196020261297061888
// 766247770432944429179173513575154591809369561091801088
// 1018517988167243043134222844204689080525734196832968125318070224677190649881668353091698688
