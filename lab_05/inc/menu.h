#ifndef MENU_H__
#define MENU_H__

typedef enum
{
    TEST_MENU_OP_EXIT,
    TEST_MENU_OP_PUSH,
    TEST_MENU_OP_POP,
    TEST_MENU_OP_SHOW,
    TEST_MENU_OP_COUNT,
    TEST_MENU_OP_UNKNOWN,
    TEST_MENU_OP_EOF,
} test_menu_op_t;

typedef enum
{
    MAIN_MENU_OP_EXIT,
    MAIN_MENU_OP_ARR_QUEUE_TEST,
    MAIN_MENU_OP_LIST_QUEUE_TEST,
    MAIN_MENU_OP_SIMULATE,
    MAIN_MENU_OP_MEASURE,
    MAIN_MENU_OP_EDIT,
    MAIN_MENU_OP_COUNT,
    MAIN_MENU_OP_UNKNOWN,
    MAIN_MENU_OP_EOF,
} main_menu_op_t;

test_menu_op_t get_test_menu_op(void);
main_menu_op_t get_main_menu_op(void);

#endif // MENU_H__
