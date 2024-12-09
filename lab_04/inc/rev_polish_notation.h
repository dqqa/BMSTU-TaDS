#ifndef REV_POLISH_NOTATION_H__
#define REV_POLISH_NOTATION_H__

#include "common.h"
#include <stdio.h>

int RPN_parse_expr(const char *expr);
int RPN_parse_expr_ex(FILE *fp);

#endif // REV_POLISH_NOTATION_H__
