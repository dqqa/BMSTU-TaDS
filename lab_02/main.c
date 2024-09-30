#include "country.h"
#include "country_array.h"
#include "errors.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EFFICIENCY_ITERS 1000

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
           "1. Вывести все записи на экран.\n"
           "2. Добавить запись в конец.\n"
           "3. Удалить запись.\n"
           "4. Просмотр отсортированной таблицы ключей при несорт. исх. табл.\n"
           "5. Вывод упорядоченной таблицы по времени полета.\n"
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
    printf("Задание:\n");
    printf("Ввести список стран, содержащий название страны, столицу, материк, необходимость\n"
           "наличия визы, время полета до страны, минимальную стоимость отдыха, основной вид туризма:\n"
           "\t1. Экскурсионный:\n"
           "\t\tКол-во объектов\n"
           "\t\tОсновной вид объектов (природа, искусство, история)\n"
           "\t2. Пляжный\n"
           "\t\tОсновной сезон\n"
           "\t\tТемпература воздуха и воды\n"
           "\t3. Спортивный\n"
           "\t\tВид спорта (горные лыжи, серфинг, восхождения)\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <DATABASE_FILE>\n", argv[0]);
        return ERR_ARGS;
    }

    print_format();

    // country_t countries[MAX_COUNTRIES] = {0};
    // В куче можно выделить существенно больше места, нежели чем на стеке, где мы ограничены ~8MiB
    country_t *countries = calloc(MAX_COUNTRIES, sizeof(country_t));
    if (!countries)
    {
        print_err(ERR_ALLOC);
        return ERR_ALLOC;
    }

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
            printf("Количество стран в файле \"%s\": %zu\n", argv[1], count);
    }
    else
    {
        perror("Невозможно открыть файл");
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
        else if (op == OP_SHOW_SORTED_TABLE)
        {
            sort_bubble(countries, count, sizeof(country_t), country_cmp_travel_time);

            printf("Таблица: \n");
            ca_print(stdout, countries, count);
        }
        else if (op == OP_SHOW_SORTED_KEY_TABLE)
        {
            key_record_t *keytable = calloc(count, sizeof(key_record_t));
            size_t keytable_size;
            if (!keytable)
            {
                rc = ERR_ALLOC;
                break;
            }
            rc = key_table_create(countries, count, keytable, count, &keytable_size, FIELD_TRAVEL_TIME);
            if (rc == ERR_OK)
            {
                sort_bubble(keytable, keytable_size, sizeof(*keytable), key_record_int_cmp);
                printf("\nОтсортированная таблица ключей:\n");
                key_table_print(keytable, keytable_size, FIELD_TRAVEL_TIME);
            }

            free(keytable);
        }
        else if (op == OP_SHOW_SORTED_TABLE_BY_KEY_TABLE)
        {
            key_record_t *keytable = calloc(count, sizeof(key_record_t));
            size_t keytable_size;
            if (!keytable)
            {
                rc = ERR_ALLOC;
                break;
            }
            rc = key_table_create(countries, count, keytable, count, &keytable_size, FIELD_TRAVEL_TIME);
            if (rc == ERR_OK)
            {
                sort_bubble(keytable, keytable_size, sizeof(*keytable), key_record_int_cmp);
                printf("\nСортированная таблица ключей:\n");
                ca_print_key(stdout, countries, count, keytable);
                // key_table_print(keytable, keytable_size, FIELD_TRAVEL_TIME);
            }

            free(keytable);
        }
        else if (op == OP_SHOW_COMPARISON_RESULTS)
        {
            country_t *new_countries = calloc(count, sizeof(country_t));
            if (!new_countries)
            {
                rc = ERR_ALLOC;
                break;
            }
            memcpy(new_countries, countries, count * sizeof(country_t));

            sort_bubble(new_countries, count, sizeof(country_t), country_cmp_travel_time);
            printf("\nРезультат сортировки алгоритмом пузырька:\n");
            ca_print(stdout, new_countries, count);

            memcpy(new_countries, countries, count * sizeof(country_t));

            qsort(new_countries, count, sizeof(country_t), country_cmp_travel_time);
            printf("\nРезультат сортировки алгоритмом qsort:\n");
            ca_print(stdout, new_countries, count);

            free(new_countries);
        }
        else if (op == OP_SHOW_COMPARISON_EFFICIENCY)
        {
            country_t *new_countries = calloc(count, sizeof(country_t));
            if (!new_countries)
            {
                rc = ERR_ALLOC;
                break;
            }
            key_record_t *keytable = calloc(count, sizeof(key_record_t));
            size_t keytable_size;
            if (!keytable)
            {
                rc = ERR_ALLOC;
                break;
            }

            float qsort_full = 0;
            for (size_t i = 0; i < EFFICIENCY_ITERS; i++)
            {
                clock_t start, end;
                memcpy(new_countries, countries, count * sizeof(country_t));
                start = clock();

                qsort(new_countries, count, sizeof(country_t), country_cmp_travel_time);

                end = clock();

                qsort_full += end - start;
            }
            qsort_full /= EFFICIENCY_ITERS;
            printf("Алгоритм сортировки qsort полной таблицы: %f us. (Микросекунд)\n", qsort_full);

            float bsort_full = 0;
            for (size_t i = 0; i < EFFICIENCY_ITERS; i++)
            {
                clock_t start, end;
                memcpy(new_countries, countries, count * sizeof(country_t));
                start = clock();

                sort_bubble(new_countries, count, sizeof(country_t), country_cmp_travel_time);

                end = clock();

                bsort_full += end - start;
            }
            bsort_full /= EFFICIENCY_ITERS;
            printf("Алгоритм сортировки пузырьком полной таблицы: %f us. (Микросекунд)\n", bsort_full);

            float qsort_key = 0;
            rc = key_table_create(countries, count, keytable, count, &keytable_size, FIELD_TRAVEL_TIME);
            for (size_t i = 0; i < EFFICIENCY_ITERS; i++)
            {
                clock_t start, end;
                memcpy(new_countries, countries, count * sizeof(country_t));
                start = clock();

                qsort(keytable, keytable_size, sizeof(*keytable), key_record_int_cmp);

                end = clock();

                qsort_key += end - start;
            }
            qsort_key /= EFFICIENCY_ITERS;
            printf("Алгоритм сортировки qsort таблицы ключей: %f us. (Микросекунд)\n", qsort_key);

            float bsort_key = 0;
            rc = key_table_create(countries, count, keytable, count, &keytable_size, FIELD_TRAVEL_TIME);
            for (size_t i = 0; i < EFFICIENCY_ITERS; i++)
            {
                clock_t start, end;
                memcpy(new_countries, countries, count * sizeof(country_t));
                start = clock();

                sort_bubble(keytable, keytable_size, sizeof(*keytable), key_record_int_cmp);

                end = clock();

                bsort_key += end - start;
            }
            bsort_key /= EFFICIENCY_ITERS;
            printf("Алгоритм сортировки пузырьком таблицы ключей: %f us. (Микросекунд)\n", bsort_key);

            printf("Сравнение эффективности работы программы:\n\n");
            printf("qsort abs: %+f ns.\nqsort rel: %+.1f%%\n\n", qsort_key - qsort_full, (qsort_key - qsort_full) / qsort_full * 100);
            printf("bsort abs: %+f ns.\nbsort rel: %+.1f%%\n\n", bsort_key - bsort_full, (bsort_key - bsort_full) / bsort_full * 100);

            free(new_countries);
            free(keytable);
        }
        else if (op == OP_EXIT)
            break;
        else
            rc = ERR_UNKNOWN_OP;

        // printf("[DEBUG] Operation %d\n", op);
    }

    if (rc != ERR_OK)
        print_err(rc);

    free(countries);
    return rc;
}
