#include "errors.h"
#include "menu.h"
#include "queue_arr.h"
#include "queue_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void queue_list_print(node_t *node, void *fmt)
{
    printf(fmt, ((data_t *)node->data)->test_num);
}

static void queue_arr_print(data_t *data, void *fmt)
{
    printf(fmt, data->test_num);
}

int test_queue_list(void)
{
    printf("\nПроверка реализации очереди на основе односвязного линейного списка.\n"
           "Для проверки, был выбран вещественный тип для хранения чисел\n");
    bool need_exit = false;
    int rc = ERR_OK;
    struct timespec start, end;

    queue_list_t q = { 0 };
    queue_list_create(&q);

    while (!need_exit)
    {
        test_menu_op_t op = get_test_menu_op();
        if (op == TEST_MENU_OP_EXIT || op == TEST_MENU_OP_EOF)
        {
            need_exit = true;
        }
        else if (op == TEST_MENU_OP_PUSH)
        {
            data_t num;
            printf("Введите число: ");
            if (scanf("%f", &num.test_num) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = queue_list_push(&q, &num, sizeof(num));
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            double t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            /* DEBUG ONLY */
            printf("Адрес конца очереди:  %p, время добавления: %.2f мкс.\n", (void *)q.end, t);
            printf("Адрес начала очереди: %p\n", (void *)q.head);
        }
        else if (op == TEST_MENU_OP_POP)
        {
            data_t num;
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            if ((rc = queue_list_pop(&q, &num, sizeof(num))) != ERR_OK)
                goto err;
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            double t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            /* DEBUG ONLY */
            printf("Число: %f\n", num.test_num);
            printf("Адрес конца очереди:  %p, время удаления: %.2f мкс.\n", (void *)q.end, t);
            printf("Адрес начала очереди: %p\n", (void *)q.head);
        }
        else if (op == TEST_MENU_OP_SHOW)
        {
            printf("Содержимое очереди:\n");
            queue_list_apply(&q, queue_list_print, "%f ");
            printf("\n");
        }
        else if (op == TEST_MENU_OP_UNKNOWN)
        {
            rc = ERR_BAD_NUM;
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

int test_queue_arr(void)
{
    printf("\nПроверка реализации очереди на основе массива.\n"
           "Ограничение: максимальное количество элементов в очереди - 100\n"
           "Для проверки, был выбран вещественный тип для хранения чисел\n");
    bool need_exit = false;
    int rc = ERR_OK;
    struct timespec start, end;

    queue_arr_t q = { 0 };
    queue_arr_create(&q);

    while (!need_exit)
    {
        test_menu_op_t op = get_test_menu_op();
        if (op == TEST_MENU_OP_EXIT || op == TEST_MENU_OP_EOF)
        {
            need_exit = true;
        }
        else if (op == TEST_MENU_OP_PUSH)
        {
            data_t num;
            printf("Введите число: ");
            if (scanf("%f", &num.test_num) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            rc = queue_arr_push(&q, &num);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            double t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            /* DEBUG ONLY */
            printf("Адрес конца очереди:  %p, время добавления: %.2f мкс.\n", (void *)(q.data + q.size), t);
            printf("Адрес начала очереди: %p\n", (void *)q.data);
        }
        else if (op == TEST_MENU_OP_POP)
        {
            data_t num;
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            if ((rc = queue_arr_pop(&q, &num)) != ERR_OK)
                goto err;
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            double t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

            /* DEBUG ONLY */
            printf("Число: %f\n", num.test_num);
            printf("Адрес конца очереди:  %p, время добавления: %.2f мкс.\n", (void *)(q.data + q.size), t);
            printf("Адрес начала очереди: %p\n", (void *)q.data);
        }
        else if (op == TEST_MENU_OP_SHOW)
        {
            printf("Содержимое очереди:\n");
            queue_arr_apply(&q, queue_arr_print, "%f ");
            printf("\n");
        }
        else if (op == TEST_MENU_OP_UNKNOWN)
        {
            rc = ERR_BAD_NUM;
        }

        err:
        if (rc != ERR_OK)
        {
            printf("Ошибка!\n");
            rc = ERR_OK;
        }
    }

    return rc;
}
