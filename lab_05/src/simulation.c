#include "simulation.h"
#include "errors.h"
#include "queue_arr.h"
#include "queue_list.h"
#include "rand_manager.h"
#include <stdio.h>

#define REPORT_INTERVAL 100

#define T1_LOWER 0
#define T1_UPPER 6

#define T2_LOWER 0
#define T2_UPPER 1

#define P 0.8f

int simulate_first_n(size_t n, queue_base_t *queue)
{
    int rc = ERR_OK;
    printf("Симуляция первых %zu заявок обслуживающего аппарата\n", n);

    size_t entered = 0, left = 0;
    size_t activations = 0;
    float current_time = 0.0f, total_idle_time = 0.0f, total_queue_time = 0.0f;
    float next_arrival_time = rand_in_range_float(T1_LOWER, T1_UPPER);
    float next_service_time = 0.0f;
    while (left < n)
    {
        if (queue->is_empty(queue) || next_arrival_time < next_service_time)
        {
            // Обработка прибытия заявки
            current_time = next_arrival_time;
            data_t req = { .req = { current_time, rand_in_range_float(T2_LOWER, T2_UPPER) } };
            if ((rc = queue->push(queue, &req) != ERR_OK))
                goto exit;

            entered++;
            next_arrival_time = current_time + rand_in_range_float(T1_LOWER, T1_UPPER);

            if (queue->size == 1) // Автомат простаивает
                next_service_time = current_time + req.req.service_time;
        }
        else
        {
            // Обработка завершения обслуживания
            if (queue->is_empty(queue))
            {
                total_idle_time += next_service_time - current_time;
                current_time = next_service_time;
                continue;
            }
            data_t req;
            if ((rc = queue->pop(queue, &req)) != ERR_OK)
                goto exit;

            // Обновляем текущее время до момента завершения обслуживания
            current_time = next_service_time;

            // total_queue_time += current_time - req.req.arrival_time; // Хз пока

            float queue_time = current_time - req.req.arrival_time;
            if (queue_time >= 0) // Проверка на отрицательное время
                total_queue_time += queue_time;
            else
                printf("Warning: отрицательное время ожидания: %.2f\n", queue_time);

            activations++;

            if (rand_with_probability(P))
            {
                // Возвращаем заявку в конец очереди
                req.req.arrival_time = current_time;
                if ((rc = queue->push(queue, &req)) != ERR_OK)
                    goto exit;
            }
            else
            {
                left++; // Заявка вышла
            }

            // Устанавливаем время следующего обслуживания
            if (!queue->is_empty(queue))
            {
                data_t next_req;
                if ((rc = queue->peek(queue, &next_req)) != ERR_OK) // Посмотреть на следующую заявку
                    goto exit;
                next_service_time = current_time + next_req.req.service_time;
            }
            else
            {
                next_service_time = next_arrival_time; // Если очередь пуста, ждем следующую заявку
            }

            // next_service_time = current_time + rand_in_range_float(T2_LOWER, T2_UPPER);

            if (left && left % REPORT_INTERVAL == 0)
            {
                printf("\nУшло %zu заявок\n", left);
                printf("Длина очереди: %zu\n", queue->size);
                printf("Средняя длина очереди: %.2f\n", (float)entered / left);
                printf("Среднее время в очереди: %.2f\n", total_queue_time / left);
            }
        }
    }

    printf("\nОбщее время моделирования: %.2f\n"
           "Время простоя ОА: %.2f\n"
           "Кол-во вошедших заявок: %zu\n"
           "Кол-во вышедших заявок: %zu\n"
           "Кол-во срабатываний ОА: %zu\n",
           current_time, total_idle_time, entered, left, activations);

    exit:
    return rc;
}
