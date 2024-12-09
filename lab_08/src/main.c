#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t vertices = 4; // Количество городов
    graph_t *graph = create_graph(vertices);

    // Добавляем дороги (пример ввода)
    add_edge(graph, 0, 1, 10);
    add_edge(graph, 1, 0, 10);
    add_edge(graph, 0, 2, 15);
    add_edge(graph, 2, 0, 15);
    add_edge(graph, 0, 3, 20);
    add_edge(graph, 3, 0, 20);
    add_edge(graph, 1, 2, 35);
    add_edge(graph, 2, 1, 35);
    add_edge(graph, 1, 3, 25);
    add_edge(graph, 3, 1, 25);
    add_edge(graph, 2, 3, 30);
    add_edge(graph, 3, 2, 30);

    // Матрица для хранения кратчайших расстояний
    int **dist = (int **)malloc(vertices * sizeof(int *));
    for (size_t i = 0; i < vertices; i++)
    {
        dist[i] = (int *)malloc(vertices * sizeof(int));
    }

    // Выполняем алгоритм Флойда-Уоршелла
    floyd_warshall(graph, dist);

    // Находим лучший город
    size_t best_city = find_best_city(dist, vertices);
    printf("Лучший город: %zu\n", best_city);

    generate_graphviz(graph, best_city, "graph.gp");
    printf("Graphviz файл создан: graph.gp\n");

    // Освобождаем память
    for (size_t i = 0; i < vertices; i++)
        free(dist[i]);

    free(dist);
    free_graph(graph);

    return 0;
}
