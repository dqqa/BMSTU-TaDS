#define _GNU_SOURCE
#include "common.h"
#include "errors.h"
#include "rev_polish_notation.h"
#include "stack_dynamic.h"
#include "stack_static.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

typedef enum
{
    OP_EXIT,
    OP_CALC_RPN,
    OP_DYN_STACK_TEST,
    OP_STATIC_STACK_TEST,
    OP_COUNT,
    OP_UNKNOWN,
    OP_EOF,
} op_t;

typedef enum
{
    TEST_OP_EXIT,
    TEST_OP_PUSH,
    TEST_OP_POP,
    TEST_OP_PRINT_CONTENTS,
    TEST_OP_COUNT,
    TEST_OP_UNKNOWN,
    TEST_OP_EOF,
} stack_test_op_t;

op_t get_mainmenu_op(void)
{
    int op, rc;

    printf("\n1. Выход\n"
           "2. Преобразовать выражение в обратную польскую нотацию\n"
           "3. Проверка работы стека, основанного на односвязном списке\n"
           "4. Проверка работы стека, основанного на массиве\n");
    printf("Введите номер операции: ");

    rc = scanf("%d", &op);
    (void)getchar();
    if (rc < 0)
        return OP_EOF;

    if (rc != 1 || op > OP_COUNT || op < 1)
        return OP_UNKNOWN;

    return (op_t)(op - 1);
}

stack_test_op_t get_stack_test_op(void)
{
    int op, rc;

    printf("\n1. Выход\n"
           "2. Добавить элемент в стек\n"
           "3. Удалить элемент из стека\n"
           "4. Вывести содержимое стека\n");
    printf("Введите номер операции: ");

    rc = scanf("%d", &op);
    (void)getchar();
    if (rc < 0)
        return TEST_OP_EOF;

    if (rc != 1 || op > TEST_OP_COUNT || op < 1)
        return TEST_OP_UNKNOWN;

    return (stack_test_op_t)(op - 1);
}

void print_guide(void)
{
    printf("Лабораторная работа №4\n"
           "Программа для проверки работы со стеком. Возможен перевод выражения из инфиксной формы в постфиксную.\n"
           "При вводе выражения, не забудьте ставить знаки пробела (' ') между операторами и операндами.\n");
}

void dyn_stack_print(node_t *node, void *fmt)
{
    printf(fmt, *(int *)node->data);
}

void static_stack_print(int *data, void *fmt)
{
    printf(fmt, *data);
}

int stack_dyn_test(void)
{
    printf("Проверка работы стека как Абстрактного Типа Данных, основанного на односвязном линейном списке\n"
           "Для проверки реализации, был выбран целый тип (int)\n\n");
    bool need_exit = false;
    int rc = ERR_OK;
    struct timespec start, end;

    stack_dyn_t stack = {0};
    stack_dyn_create(&stack);

    while (!need_exit)
    {
        stack_test_op_t op = get_stack_test_op();
        if (op == TEST_OP_EOF || op == TEST_OP_EXIT)
        {
            need_exit = true;
        }
        else if (op == TEST_OP_PUSH)
        {
            printf("Введите число: ");
            int number;
            if (scanf("%d", &number) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = stack_dyn_push(&stack, &number, sizeof(number));
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            if (rc != ERR_OK)
                goto err;

            /* DEBUG ONLY */
            const void *new_addr = stack.head;
            double elapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
            printf("[ОТЛАДКА] Адрес для нового элемента в стеке: %p. Время: %.2f мкс.\n", new_addr, elapsed);
        }
        else if (op == TEST_OP_POP)
        {
            int number;
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = stack_dyn_pop(&stack, &number, sizeof(number));
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            if (rc != ERR_OK)
                goto err;

            printf("Значение с верхушки стека: %d\n", number);

            /* DEBUG ONLY */
            const void *new_addr = stack.head;
            double elapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
            printf("[ОТЛАДКА] Адрес для предыдущего элемента в стеке: %p. Время: %.2f мкс.\n", new_addr, elapsed);
        }
        else if (op == TEST_OP_PRINT_CONTENTS)
        {
            printf("Содержимое стека: ");
            stack_dyn_apply(&stack, dyn_stack_print, "%d ");
            printf("\n");
        }
        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка (%d)\n", rc);
            rc = ERR_OK;
        }
    }

    stack_dyn_free(&stack);
    return rc;
}

int stack_static_test(void)
{
    printf("Проверка работы стека как Абстрактного Типа Данных, основанного на статическом массиве\n"
           "Для проверки реализации, был выбран целый тип (int)\n\n");
    bool need_exit = false;
    int rc = ERR_OK;
    struct timespec start, end;

    stack_static_t stack = {0};
    stack_static_create(&stack);

    while (!need_exit)
    {
        stack_test_op_t op = get_stack_test_op();
        if (op == TEST_OP_EOF || op == TEST_OP_EXIT)
        {
            need_exit = true;
        }
        else if (op == TEST_OP_PUSH)
        {
            printf("Введите число: ");
            int number;
            if (scanf("%d", &number) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = stack_static_push(&stack, number);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            if (rc != ERR_OK)
                goto err;

            /* DEBUG ONLY */
            const void *new_addr = stack.data + stack.top;
            double elapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
            printf("[ОТЛАДКА] Адрес для нового элемента в стеке: %p. Время: %.2f мкс.\n", new_addr, elapsed);
        }
        else if (op == TEST_OP_POP)
        {
            int number;
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = stack_static_pop(&stack, &number);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            if (rc != ERR_OK)
                goto err;

            printf("Значение с верхушки стека: %d\n", number);

            /* DEBUG ONLY */
            const void *new_addr = stack.data + stack.top;
            double elapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
            printf("[ОТЛАДКА] Адрес для предыдущего элемента в стеке: %p. Время: %.2f мкс.\n", new_addr, elapsed);
        }
        else if (op == TEST_OP_PRINT_CONTENTS)
        {
            printf("Содержимое стека: ");
            stack_static_apply(&stack, static_stack_print, "%d ");
            printf("\n");
        }
        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка (%d)\n", rc);
            rc = ERR_OK;
        }
    }

    return rc;
}

int main(void)
{
    print_guide();
    int rc = ERR_OK;
    bool need_to_exit = false;

    while (!need_to_exit)
    {
        op_t cur_op = get_mainmenu_op();
        if (cur_op == OP_EXIT || cur_op == OP_EOF)
        {
            need_to_exit = true;
        }
        else if (cur_op == OP_CALC_RPN)
        {
            printf("Введите выражение в инфиксной форме: ");
            rc = RPN_parse_expr_ex(stdin);
        }
        else if (cur_op == OP_DYN_STACK_TEST)
        {
            rc = stack_dyn_test();
        }
        else if (cur_op == OP_STATIC_STACK_TEST)
        {
            rc = stack_static_test();
        }

        if (rc != ERR_OK)
        {
            printf("Возникла ошибка!\n");
            rc = ERR_OK;
        }
    }

    return rc;
}
