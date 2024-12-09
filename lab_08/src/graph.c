#include "graph.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INF INT_MAX // Используем для обозначения бесконечности

// Функция для создания графа
graph_t *create_graph(size_t vert_cnt)
{
    graph_t *graph = malloc(sizeof(graph_t));
    graph->vertices_cnt = vert_cnt;
    graph->matrix = malloc(vert_cnt * sizeof(int *));
    for (size_t i = 0; i < vert_cnt; i++)
    {
        graph->matrix[i] = malloc(vert_cnt * sizeof(int));
        for (size_t j = 0; j < vert_cnt; j++)
        {
            graph->matrix[i][j] = (i == j) ? 0 : INF; // 0 на диагонали, INF в остальных местах
        }
    }
    return graph;
}

// Функция для добавления ребра
void add_edge(graph_t *graph, size_t src, size_t dest, size_t weight)
{
    graph->matrix[src][dest] = weight;
}

// Функция для освобождения памяти графа
void free_graph(graph_t *graph)
{
    for (size_t i = 0; i < graph->vertices_cnt; i++)
        free(graph->matrix[i]);

    free(graph->matrix);
    free(graph);
}

// Алгоритм Флойда-Уоршелла
void floyd_warshall(graph_t *graph, int **dist)
{
    size_t v = graph->vertices_cnt;
    for (size_t i = 0; i < v; i++)
        for (size_t j = 0; j < v; j++)
            dist[i][j] = graph->matrix[i][j];

    for (size_t k = 0; k < v; k++)
        for (size_t i = 0; i < v; i++)
            for (size_t j = 0; j < v; j++)
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][j] > dist[i][k] + dist[k][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}

// Функция для нахождения города с минимальной суммой расстояний
size_t find_best_city(int **dist, size_t vertices)
{
    int min_sum = INF;
    int best_city = -1;

    for (size_t i = 0; i < vertices; i++)
    {
        int sum = 0;
        for (size_t j = 0; j < vertices; j++)
            sum += dist[i][j];

        if (sum < min_sum)
        {
            min_sum = sum;
            best_city = i;
        }
    }

    return best_city;
}

// Функция для генерации исходного текста для Graphviz
void generate_graphviz(graph_t *graph, size_t best_city, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Не удалось открыть файл");
        return;
    }

    fprintf(file, "digraph G {\n");

    for (size_t i = 0; i < graph->vertices_cnt; i++)
        for (size_t j = 0; j < graph->vertices_cnt; j++)
            if (graph->matrix[i][j] != INF && graph->matrix[i][j] != 0)
                fprintf(file, "    %zu -> %zu [label=\"%d\"];\n", i, j, graph->matrix[i][j]);

    // Выделяем лучший город
    fprintf(file, "    %zu [style=filled, fillcolor=yellow];\n", best_city);
    fprintf(file, "}\n");

    fclose(file);
}

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
