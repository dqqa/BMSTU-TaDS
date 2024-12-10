#include "errors.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_guide(void)
{
    printf("Программа ищет узел, имеющий минимальную сумму путей до всех остальных.\n"
           "В первой строке файла должно быть кол-во узлов (городов)\n"
           "На последующих N строках по вертикали - исходные города, по горизонтали - конечные города.\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_guide();
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return ERR_ARGS;
    }

    struct timespec start, end;
    graph_t *graph = NULL;
    int **dist = NULL;
    size_t vertices = 0;

    int rc = graph_load_ex(argv[1], &graph);
    if (rc != ERR_OK)
        goto err;

    vertices = graph->vertices_cnt;

    // Матрица для хранения кратчайших расстояний
    dist = (int **)malloc(vertices * sizeof(int *));
    for (size_t i = 0; i < vertices; i++)
    {
        dist[i] = (int *)malloc(vertices * sizeof(int));
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Выполняем алгоритм Флойда-Уоршелла
    floyd_warshall(graph, dist);

    // Находим лучший город
    size_t best_city = find_best_city(dist, vertices);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    float t = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

    printf("Лучший город: %zu\n", best_city);

    printf("Использование памяти графом: %zu байт\n", graph_calc_ram_usage(graph));
    printf("Время поиска: %.2f мкс.\n", t);

    generate_graphviz(graph, best_city, "graph2.gp");
    rc = graph_render_open("graph2.gp", "graph2.png");
    // printf("Graphviz файл создан: graph.gp\n");

    err:
    // Освобождаем память
    for (size_t i = 0; i < vertices; i++)
        free(dist[i]);

    free(dist);
    free_graph(graph);

    return 0;
}
