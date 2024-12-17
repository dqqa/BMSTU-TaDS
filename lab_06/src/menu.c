#include "mymenu.h"
#include <stdio.h>

main_menu_op_t get_main_menu_op(void)
{
    printf("\n1. Выход\n"
           "2. Проверка работы дерева слов\n"
           "3. Загрузить данные из файла\n"
           "4. Поиск слов, начинающихся с введенной буквы\n"
           "5. Сравнение производительности\n");
    printf("Введите номер операции: ");

    int num;
    int rc = scanf("%d", &num);
    if (rc < 0)
        return MAIN_MENU_EOF;
    if (rc == 0)
    {
        while (getchar() != '\n')
            ;
        return MAIN_MENU_UNKNOWN;
    }

    getchar();

    if (num < 1 || num > MAIN_MENU_COUNT)
        return MAIN_MENU_UNKNOWN;

    return (main_menu_op_t)(num - 1);
}

test_menu_op_t get_test_menu_op(void)
{
    printf("\n1. Выход\n"
           "2. Добавить элемент в дерево\n"
           "3. Удалить элемент из дерева\n"
           "4. Поиск элемента в дереве\n"
           "5. Просмотр дерева в формате DOT\n"
           "6. Префиксный обход дерева\n"
           "7. Инфиксный обход дерева\n"
           "8. Постфиксный обход дерева\n");
    printf("Введите номер операции: ");

    int num;
    int rc = scanf("%d", &num);
    if (rc < 0)
        return TEST_MENU_EOF;
    if (rc == 0)
    {
        while (getchar() != '\n')
            ;

        return TEST_MENU_UNKNOWN;
    }

    getchar();

    if (num < 1 || num > TEST_MENU_COUNT)
        return TEST_MENU_UNKNOWN;

    return (test_menu_op_t)(num - 1);
}
