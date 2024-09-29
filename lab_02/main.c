#include "country.h"
#include "errors.h"
#include <stdio.h>

int main(void)
{
    country_t c[30] = {0};
    FILE *fp = fopen("./data/1.txt", "r");
    int rc = ERR_OK;
    size_t count = 0;
    while (1)
    {
        if (count >= 30)
            break;
        rc = country_read(fp, c + count);
        if (rc != ERR_OK)
            break;
        count++;
    }
    printf("COUNT: %zu\n", count);
    for (size_t i=0;i<count;i++)
        country_print(stdout, c+i);
    return 0;
}
