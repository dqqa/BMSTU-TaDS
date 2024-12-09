#include "graph.h"
#include "errors.h"
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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

int graph_render_open(const char *gp_fname, const char *png_fname)
{
    pid_t pid = fork();
    if (pid == -1)
        return ERR_FORK;

    if (pid == 0)
    {
        execlp("dot", "dot", "-Tpng", gp_fname, "-o", png_fname, NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    else
    {
        int ret_code;
        wait(&ret_code);

        if (WEXITSTATUS(ret_code) != 0)
            return ERR_FORK;
    }

    pid = fork();
    if (pid == -1)
        return ERR_FORK;

    if (pid == 0)
    {
        int new_stderr = open("/dev/null", O_WRONLY);
        if (new_stderr < 0)
            return ERR_FORK;

        dup2(new_stderr, STDERR_FILENO);

        execlp("open", "open", png_fname, NULL);
        perror("execlp");
        close(new_stderr);
        _exit(EXIT_FAILURE);
    }
    else
    {
        int ret_code;
        wait(&ret_code);

        if (WEXITSTATUS(ret_code) != 0)
            return ERR_FORK;
    }

    return ERR_OK;
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

int graph_load(FILE *fp, graph_t **graph)
{
    int rc = ERR_OK;
    size_t verts;
    if (fscanf(fp, "%zu", &verts) != 1)
        return ERR_IO;

    *graph = create_graph(verts);
    if (*graph == NULL)
        return ERR_ALLOC;

    for (size_t i = 0; i < verts; i++)
    {
        for (size_t j = 0; j < verts; j++)
        {
            int weight;
            if (fscanf(fp, "%d", &weight) != 1)
            {
                rc = ERR_IO;
                goto err;
            }
            add_edge(*graph, j, i, weight);
        }
    }

    err:
    if (rc != ERR_OK)
        free_graph(*graph);

    return rc;
}

int graph_load_ex(const char *filename, graph_t **graph)
{
    FILE *fp = fopen(filename, "r");

    int rc = graph_load(fp, graph);

    fclose(fp);
    return rc;
}

size_t graph_calc_ram_usage(const graph_t *graph)
{
    if (graph == NULL)
        return 0;

    return graph->vertices_cnt * graph->vertices_cnt * sizeof(**graph->matrix) + sizeof(*graph);
}
