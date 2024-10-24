#ifndef COMMON_H__
#define COMMON_H__

#define DATA_PRI "f"
#define DATA_SCN "f"
typedef float data_t;

typedef enum __math_op_t
{
    MATH_ADD,
    MATH_SUB,
    MATH_MUL,
    MATH_DIV,
    MATH_OPEN_PAREN,
    MATH_CLOSE_PAREN,
    MATH_COUNT,
    MATH_UNKNOWN,
} math_op_t;

typedef struct
{
    math_op_t op;
    const char *str;
} match_t;

extern match_t op_matches[];

#endif // COMMON_H__
