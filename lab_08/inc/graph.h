#ifndef GRAPH_H__
#define GRAPH_H__

#include <stddef.h>

// Структура для хранения графа в виде матрицы смежности
typedef struct __graph_t
{
    size_t vertices_cnt; // Количество вершин
    int **matrix; // Матрица смежности
} graph_t;

graph_t *create_graph(size_t vert_cnt);
void free_graph(graph_t *graph);

void add_edge(graph_t *graph, size_t src, size_t dest, size_t weight);

void floyd_warshall(graph_t *graph, int **dist);
size_t find_best_city(int **dist, size_t vertices);

void generate_graphviz(graph_t *graph, size_t best_city, const char *filename);

#endif // GRAPH_H__
