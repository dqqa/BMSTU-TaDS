#define _GNU_SOURCE

#include "simulation.h"
#include "errors.h"
#include "queue_arr.h"
#include "queue_list.h"
#include "rand_manager.h"
#include <stdio.h>
#include <time.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define REPORT_INTERVAL 100

#define T1_LOWER 0
#define T1_UPPER 6

#define T2_LOWER 0
#define T2_UPPER 1

#define P 0.8f

int simulate_first_n(size_t n, queue_base_t *queue, float *time)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    int rc = ERR_OK;
    printf("Симуляция первых %zu заявок обслуживающего аппарата\n", n);

    size_t entered = 0, left = 0, prev_left = 0;
    size_t activations = 0;
    float current_time = 0.0f, total_idle_time = 0.0f, total_queue_time = 0.0f, total_service_time = 0.0f;
    float next_arrival_time = rand_in_range_float(T1_LOWER, T1_UPPER); // Время прибытия следующей заявки
    float next_service_time = 0.0f;                                    // Время следющего обслуживания
    while (left < n)
    {
        if (queue->is_empty(queue) || next_arrival_time < next_service_time)
        {
            if (queue->is_empty(queue))
            {
                // Если очередь пуста, аппарат простаивает до следующего прибытия
                total_idle_time += next_arrival_time - current_time;
            }

            // Обработка прибытия заявки
            current_time = next_arrival_time;
            data_t req = { .req = { .arrival_time = current_time, .service_time = rand_in_range_float(T2_LOWER, T2_UPPER) } };
            if ((rc = queue->push(queue, &req) != ERR_OK))
                goto exit;

            entered++;
            next_arrival_time = current_time + rand_in_range_float(T1_LOWER, T1_UPPER);

            if (queue->size != 0) // Автомат простаивает
                next_service_time = current_time + req.req.service_time;
        }
        else
        {
            // Обработка завершения обслуживания
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
                // entered++;
                req.req.arrival_time = current_time;
                // req.req.service_time = rand_in_range_float(T2_LOWER, T2_UPPER); // Генерация нового времени обслуживания не требуется
                if ((rc = queue->push(queue, &req)) != ERR_OK)
                    goto exit;
            }
            else
            {
                total_service_time += req.req.service_time;
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

            if (left && left % REPORT_INTERVAL == 0 && left != prev_left)
            {
                prev_left = left;
                printf("\nУшло %zu заявок\n", left);
                printf("Длина очереди: %zu\n", queue->size);
                printf("TODO Средняя длина очереди: %.2f\n", (float)entered / left);
                printf("Среднее время в очереди: %.2f\n", total_queue_time / left);
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    *time = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

    printf("\nОбщее время моделирования: %.2f\n"
           "Время простоя ОА: %.2f\n"
           "Время обслуживания ОА: %.2f\n"
           "Кол-во вошедших заявок: %zu\n"
           "Кол-во вышедших заявок: %zu\n"
           "Кол-во срабатываний ОА: %zu\n",
           current_time, total_idle_time, total_service_time, entered, left, activations);

    float t_avg_wait_req = (float)(T1_UPPER - T1_LOWER) / 2;
    float t_avg_serv_req = (float)(T2_UPPER - T2_LOWER) / 2;
    float calculated_req_cnt = current_time / t_avg_wait_req;
    float calculated_time = activations * t_avg_serv_req + total_idle_time;
    printf("\nВычисление погрешностей.\n"
           "Вычисленное кол-во заявок: %.2f, имеем: %zu, погрешность: %.2f%%\n"
           "Аппарат работал: %zu, простаивал: %.2f\n"
           "Значит время моделирования должно быть: %.2f, имеем %.2f\n"
           "Вычисляем погрешность: %.2f%%\n",
           calculated_req_cnt, entered, ABS((entered - calculated_req_cnt) / calculated_req_cnt * 100),
           activations, total_idle_time,
           calculated_time, current_time,
           ABS((current_time - calculated_time) / calculated_time * 100));

    exit:
    return rc;
}
