#include "country.h"
#include "country_array.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum __operation_t
{
    OP_SHOW_TABLE,
    OP_APPEND,
    OP_REMOVE,
    OP_SHOW_SORTED_KEY_TABLE,
    OP_SHOW_SORTED_TABLE,
    OP_SHOW_SORTED_TABLE_BY_KEY_TABLE,
    OP_SHOW_COMPARISON_RESULTS,
    OP_SHOW_COMPARISON_EFFICIENCY,
    OP_EXIT,
    OP_UNKNOWN,
    OP_COUNT
} operation_t;

operation_t get_menu_choice(void)
{
    printf("\n"
           "1. Вывести все записи на экран\n"
           "2. Добавить запись в конец\n"
           "3. Удалить запись\n"
           "4. Просмотр отсортированной таблицы ключей при несорт. исх. табл.\n"
           "5. Вывод упорядоченной таблицы по ключу\n"
           "6. Вывод исходной таблицы в упорядоченном виде, используя упоряд. табл. ключей\n"
           "7. Вывод результатов исп. различных сортровок.\n"
           "8. Вывод результатов сравнения эфф. работы программы.\n"
           "9. Выйти.\n");

    printf("Введите операцию: ");
    int op;
    if (scanf("%d", &op) != 1)
        return OP_UNKNOWN;
    getchar();

    if (op > OP_COUNT || op <= 0)
        return OP_UNKNOWN;

    return (operation_t)(op - 1);
}

void print_format(void)
{
    printf("TODO");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <DATABASE_FILE>\n", argv[0]);
        return ERR_ARGS;
    }

    // country_t countries[MAX_COUNTRIES] = {0};
    // В куче можно выделить существенно больше места, нежели чем на стеке, где мы ограничены ~8MiB
    country_t *countries = calloc(MAX_COUNTRIES, sizeof(country_t));

    size_t count;

    FILE *fp = fopen(argv[1], "r");
    int rc = ERR_OK;

    if (fp)
    {
        // rc = ca_input(fp, countries, ARR_LEN(countries), &count);
        rc = ca_input(fp, countries, MAX_COUNTRIES, &count);
        fclose(fp);

        if (rc != ERR_OK)
            print_err(rc);
        else
            printf("COUNT: %zu\n", count);
    }
    else
    {
        perror("Unable to open file");
        rc = ERR_IO;
    }
    while (rc == ERR_OK)
    {
        operation_t op = get_menu_choice();
        if (op == OP_REMOVE)
        {
            size_t ind;
            printf("Введите индекс записи для удаления: ");
            if (scanf("%zu", &ind) != 1)
                rc = ERR_IO;

            if (rc == ERR_OK)
                ca_remove(countries, &count, ind);
        }
        else if (op == OP_APPEND)
        {
            rc = ca_append(stdin, countries, MAX_COUNTRIES, &count);
        }
        else if (op == OP_SHOW_TABLE)
        {
            printf("Таблица: \n");
            ca_print(stdout, countries, count);
        }
        else if (op == OP_EXIT)
            break;
        else
            rc = ERR_UNKNOWN_OP;

        printf("[DEBUG] Operation %d\n", op);
    }

    if (rc != ERR_OK)
        print_err(rc);

    free(countries);
    return rc;
}
