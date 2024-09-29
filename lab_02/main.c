#include "country.h"
#include "country_array.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <DATABASE_FILE>\n", argv[0]);
        return ERR_ARGS;
    }

    // country_t countries[MAX_COUNTRIES] = {0};
    // В куче можно выделить существенно больше места, нежели чем на стеке, где мы ограничены ~8MiB
    country_t *countries = calloc(MAX_COUNTRIES, sizeof(country_t));

    size_t count;

    FILE *fp = fopen(argv[1], "r");
    int rc = ERR_OK;

    if (fp)
    {
        // rc = ca_input(fp, countries, ARR_LEN(countries), &count);
        rc = ca_input(fp, countries, MAX_COUNTRIES, &count);
        fclose(fp);

        printf("COUNT: %zu\n", count);
        if (rc == ERR_OK)
            ca_print(stdout, countries, count);
        else
            print_err(rc);
    }
    else
    {
        perror("Unable to open file");
        rc = ERR_IO;
    }
    free(countries);
    return rc;
}
