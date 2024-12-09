#include "errors.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return ERR_ARGS;
    }

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

    // Выполняем алгоритм Флойда-Уоршелла
    floyd_warshall(graph, dist);

    // Находим лучший город
    size_t best_city = find_best_city(dist, vertices);
    printf("Лучший город: %zu\n", best_city);

    printf("Использование памяти графом: %zu байт\n", graph_calc_ram_usage(graph));

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
