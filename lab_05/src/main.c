#include "errors.h"
#include "queue_arr.h"
#include "queue_list.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    TEST_MENU_OP_EXIT,
    TEST_MENU_OP_PUSH,
    TEST_MENU_OP_POP,
    TEST_MENU_OP_SHOW,
    TEST_MENU_OP_COUNT,
    TEST_MENU_OP_UNKNOWN,
    TEST_MENU_OP_EOF,
} test_menu_t;

void apply_fn_arr(data_t *node, void *fmt)
{
    printf(fmt, *node);
}

test_menu_t get_test_menu_op(void)
{
    printf("1. Выход\n"
           "2. Добавить элемент в начало очереди\n"
           "3. Удалить элемент из конца очереди\n"
           "4. Вывести очередь\n");
    printf("Введите операцию: ");
    int num;
    int rc = scanf("%d", &num);
    if (rc == -1)
        return TEST_MENU_OP_EOF;

    if (rc <= 0 || rc > TEST_MENU_OP_COUNT)
        return TEST_MENU_OP_UNKNOWN;

    return (test_menu_t)(num - 1);
}

int test_queue_list(void)
{
    printf("Проверка реализации очереди на основе односвязного линейного списка.\n"
           "Для проверки, был выбран целый тип для хранения чисел\n");
    bool need_exit = false;
    int rc = ERR_OK;
    queue_list_t q = { 0 };
    queue_list_create(&q);

    while (!need_exit)
    {
        test_menu_t op = get_test_menu_op();
        if (op == TEST_MENU_OP_EXIT || op == TEST_MENU_OP_EOF)
        {
            need_exit = true;
        }
        else if (op == TEST_MENU_OP_PUSH)
        {
            int num;
            if (scanf("%d", &num) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            rc = queue_list_push(&q, &num, sizeof(num));
        }
        else if (op == TEST_MENU_OP_POP)
        {
            int num;
            if ((rc = queue_list_pop(&q, &num, sizeof(num))) != ERR_OK)
                goto err;

            printf("Число: %d\n", num);
        }
        else if (op == TEST_MENU_OP_SHOW)
        {
            printf("Содержимое очереди:\n");
            int rc = ERR_OK;
            int num;
            while ((rc = queue_list_pop(&q, &num, sizeof(num))) == ERR_OK)
                printf("%d ", num);
            printf("\n");
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }
    queue_list_free(&q);

    return rc;
}

int main(void)
{
    test_queue_list();
    return 0;
}
