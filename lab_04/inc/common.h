#ifndef COMMON_H__
#define COMMON_H__

#define DATA_PRI "d"
#define DATA_SCN "d"
typedef int data_t;

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
    int priority;
    char symbol;
} math_op_ex_t;

extern math_op_ex_t operations[];

#endif // COMMON_H__
