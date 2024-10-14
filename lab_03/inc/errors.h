#ifndef ERRORS_H__
#define ERRORS_H__

#define ERR_OK 0
#define ERR_IO 1
#define ERR_ALLOC 2
#define ERR_RANGE 3
#define ERR_PARAMS 4
#define ERR_NOT_FOUND 5
#define ERR_ARGS 6
#define ERR_OVERFLOW 7

void print_err(int rc);

#endif // ERRORS_H__
