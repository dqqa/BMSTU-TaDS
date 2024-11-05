#define _GNU_SOURCE

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

void compare_queue_impl(void)
{
    queue_list_t queue_lst;
    queue_arr_t queue_arr;

    queue_list_create(&queue_lst);
    queue_arr_create(&queue_arr);

    data_t test_data = { .test_num = 123 };

    float lst_avg_push = 0, arr_avg_push = 0;
    float lst_avg_pop = 0, arr_avg_pop = 0;

    for (size_t i = 0; i < 100; i++)
    {
        struct timespec start, end;
        /* LIST IMPL */
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        queue_lst.base.push(&queue_lst, &test_data);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        lst_avg_push += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

        data_t tmp;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        queue_lst.base.pop(&queue_lst, &tmp);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        lst_avg_pop += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

        /* ARRAY IMPL */
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        queue_arr.base.push(&queue_arr, &test_data);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        arr_avg_push += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        queue_arr.base.pop(&queue_arr, &tmp);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        arr_avg_pop += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
    }
    lst_avg_push /= 100;
    lst_avg_pop /= 100;
    arr_avg_push /= 100;
    arr_avg_pop /= 100;

    queue_list_free(&queue_lst);

    printf("\n==== РЕЗУЛЬТАТЫ ЗАМЕРОВ ====\n");
    printf("Очередь на основе односвязного списка:\n"
           "  PUSH: %.2f мкс.\n"
           "  POP: %.2f мкс.\n"
           "Очередь на основе массива:\n"
           "  PUSH: %.2f мкс.\n"
           "  POP: %.2f мкс.\n",
           lst_avg_push, lst_avg_pop,
           arr_avg_push, arr_avg_pop);
}
