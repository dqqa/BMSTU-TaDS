#include "common.h"
#include "errors.h"
#include "rev_polish_notation.h"
#include "stack_dynamic.h"
#include "stack_static.h"
#include <stdio.h>

int main(void)
{
    int rc = ERR_OK;
    FILE *fp = fopen("tests/test_01.txt", "r");
    if (!fp)
    {
        rc = ERR_IO;
        goto cleanup;
    }

    rc = RPN_parse_expr_ex(fp);

    cleanup:
    if (fp)
        fclose(fp);

    return rc;
}
