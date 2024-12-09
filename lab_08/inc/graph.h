#ifndef GRAPH_H__
#define GRAPH_H__

#include <stddef.h>

// Структура для хранения графа в виде матрицы смежности
typedef struct __graph_t
{
    size_t vertices_cnt; // Количество вершин
    int **matrix; // Матрица смежности
} graph_t;

#endif // GRAPH_H__
