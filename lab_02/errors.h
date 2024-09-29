#ifndef ERRORS_H_
#define ERRORS_H_

#define ERR_OK 0
#define ERR_IO 1
#define ERR_OVERFLOW 2
#define ERR_UNKNOWN_OP 3
#define ERR_UNKNOWN_TYPE 4

void print_err(int err);

#endif // ERRORS_H_
