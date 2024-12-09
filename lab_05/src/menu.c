#include "menu.h"
#include "errors.h"
#include "simulation.h"
#include <stdio.h>

test_menu_op_t get_test_menu_op(void)
{
    printf("\n1. Выход\n"
           "2. Добавить элемент в начало очереди\n"
           "3. Удалить элемент из конца очереди\n"
           "4. Вывести очередь\n");
    printf("Введите операцию: ");
    int num;
    int rc = scanf("%d", &num);
    if (rc == -1)
        return TEST_MENU_OP_EOF;

    if (rc != 1)
    {
        while (getchar() != '\n')
            ;
        return TEST_MENU_OP_UNKNOWN;
    }

    if (num <= 0 || num > TEST_MENU_OP_COUNT)
        return TEST_MENU_OP_UNKNOWN;

    return (test_menu_op_t)(num - 1);
}

main_menu_op_t get_main_menu_op(void)
{
    printf("\n1. Выход\n"
           "2. Проверка работы очереди на массиве\n"
           "3. Проверка работы очереди на односвязном списке\n"
           "4. Симуляция работы ОА\n"
           "5. Провести измерения времени работы с очередями с различной реализацией\n"
           "6. Изменить параметры\n");
    printf("Введите операцию: ");
    int num;
    int rc = scanf("%d", &num);
    if (rc == -1)
        return MAIN_MENU_OP_EOF;

    if (rc != 1)
    {
        while (getchar() != '\n')
            ;
        return MAIN_MENU_OP_UNKNOWN;
    }

    if (num <= 0 || num > MAIN_MENU_OP_COUNT)
        return MAIN_MENU_OP_UNKNOWN;

    return (main_menu_op_t)(num - 1);
}

int edit_params(void)
{
    int t1_low, t1_high;
    int t2_low, t2_high;
    float p;
    printf("Введите минимум и максимум T1 (изначально 0, 6): ");
    if (scanf("%d%d", &t1_low, &t1_high) != 2)
        return ERR_IO;

    printf("Введите минимум и максимум T2 (изначально 0, 1): ");
    if (scanf("%d%d", &t2_low, &t2_high) != 2)
        return ERR_IO;

    printf("Введите вероятность повтороного попадания в очередь (изначально 0.8): ");
    if (scanf("%f", &p) != 1)
        return ERR_IO;

    T1_LOWER = t1_low;
    T1_UPPER = t1_high;
    T2_LOWER = t2_low;
    T2_UPPER = t2_high;
    P = p;
    
    return ERR_OK;
}
