#ifndef STR_H_
#define STR_H_

#include <stdio.h>

int str_input(FILE *fp, char *buf, size_t max_buf_size);
size_t str_strip(char *str);

#endif // STR_H_
