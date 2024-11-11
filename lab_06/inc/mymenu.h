#ifndef MYMENU_H__
#define MYMENU_H__

typedef enum __main_menu_op_t
{
    MAIN_MENU_EXIT,
    MAIN_MENU_TEST_TREE,
    MAIN_MENU_LOAD,
    MAIN_MENU_SEARCH,
    MAIN_MENU_PERFORMANCE_CMP,
    MAIN_MENU_COUNT,
    MAIN_MENU_UNKNOWN,
    MAIN_MENU_EOF
} main_menu_op_t;

typedef enum __test_menu_op_t
{
    TEST_MENU_EXIT,
    TEST_MENU_ADD,
    TEST_MENU_REMOVE,
    TEST_MENU_SEARCH,
    TEST_MENU_SHOW,
    TEST_MENU_COUNT,
    TEST_MENU_UNKNOWN,
    TEST_MENU_EOF
} test_menu_op_t;

main_menu_op_t get_main_menu_op(void);
test_menu_op_t get_test_menu_op(void);

#endif // MYMENU_H__
