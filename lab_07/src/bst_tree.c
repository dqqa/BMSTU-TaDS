#define _GNU_SOURCE

#include "bst_tree.h"
#include "dyn_array.h"
#include "errors.h"
#include "str.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define UNUSED(x) ((void)(x))
#define NOTHING
#define UNREACHABLE(msg)        \
    do                          \
    {                           \
        fprintf(stderr, (msg)); \
        exit(EXIT_FAILURE);     \
    } while (0)

int bst_create(bst_tree_t **t, const char *data)
{
    int rc = ERR_OK;
    *t = NULL;

    bst_tree_t *tree = malloc(sizeof(*tree));
    if (!tree)
        return ERR_ALLOC;

    tree->is_repeated = false;
    tree->lhs = NULL;
    tree->rhs = NULL;
    tree->data = NULL;

    tree->data = strdup(data);
    if (!tree->data)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    err:
    if (rc != ERR_OK)
        free(tree);
    else
        *t = tree;

    return rc;
}

void bst_free(bst_tree_t *tree)
{
    if (!tree)
        return;

    bst_free(tree->lhs);
    bst_free(tree->rhs);

    free(tree->data);
    free(tree);
}

void bst_apply_pre(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    apply_fn(tree, arg);
    bst_apply_pre(tree->lhs, apply_fn, arg);
    bst_apply_pre(tree->rhs, apply_fn, arg);
}

void bst_apply_in(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    bst_apply_in(tree->lhs, apply_fn, arg);
    apply_fn(tree, arg);
    bst_apply_in(tree->rhs, apply_fn, arg);
}

void bst_apply_post(bst_tree_t *tree, bst_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    bst_apply_post(tree->lhs, apply_fn, arg);
    bst_apply_post(tree->rhs, apply_fn, arg);
    apply_fn(tree, arg);
}

bst_tree_t *bst_insert_node(bst_tree_t *tree, bst_tree_t *src, int *err)
{
    if (!tree)
        return src;

    *err = ERR_OK;

    int cmp_res = strcmp(tree->data, src->data);
    if (cmp_res == 0)
        *err = ERR_REPEAT; // assert(0 && "Yet there is no support of repeating nodes!");
    else if (cmp_res < 0)
        tree->rhs = bst_insert_node(tree->rhs, src, err);
    else
        tree->lhs = bst_insert_node(tree->lhs, src, err);

    return tree;
}

int bst_insert_str(bst_tree_t **tree, const char *src)
{
    bst_tree_t *subtree;
    int rc = bst_create(&subtree, src);
    if (rc != ERR_OK)
        return rc;

    bst_tree_t *newtree = bst_insert_node(*tree, subtree, &rc);
    if (rc != ERR_OK)
        goto err;

    *tree = newtree;

    err:
    if (rc != ERR_OK)
        bst_free(subtree);

    return rc;
}

static bst_tree_t *tree_min_node(bst_tree_t *node)
{
    bst_tree_t *current = node;
    while (current && current->lhs != NULL)
    {
        current = current->lhs;
    }
    return current;
}

int bst_remove(bst_tree_t **tree, bst_tree_t *what)
{
    if (*tree == NULL)
        return ERR_PARAM;

    int cmp = strcmp((*tree)->data, what->data);
    if (cmp > 0)
        return bst_remove(&(*tree)->lhs, what);
    else if (cmp < 0)
        return bst_remove(&(*tree)->rhs, what);
    else
    {
        if ((*tree)->lhs == NULL)
        {
            bst_tree_t *temp = (*tree)->rhs;
            free((*tree)->data);
            free(*tree);
            *tree = temp;
        }
        else if ((*tree)->rhs == NULL)
        {
            bst_tree_t *temp = (*tree)->lhs;
            free((*tree)->data);
            free(*tree);
            *tree = temp;
        }
        else
        {
            bst_tree_t *temp = tree_min_node((*tree)->rhs);
            free((*tree)->data);
            (*tree)->data = strdup(temp->data);
            (*tree)->is_repeated = temp->is_repeated;
            bst_remove(&(*tree)->rhs, temp);
        }
        return ERR_OK;
    }
}

int bst_remove_str(bst_tree_t **tree, const char *what)
{
    bst_tree_t *node_to_remove = bst_search(*tree, what, NULL);
    if (!node_to_remove)
        return ERR_NOT_FOUND;

    return bst_remove(tree, node_to_remove);
}

bst_tree_t *bst_search(bst_tree_t *tree, const char *data, size_t *cmps)
{
    if (!tree)
        return NULL;

    if (cmps)
        (*cmps)++;

    int cmp_res = strcmp(data, tree->data);
    if (cmp_res == 0)
    {
        tree->is_repeated = true;
        return tree;
    }
    else if (cmp_res < 0)
        return bst_search(tree->lhs, data, cmps);
    else
        return bst_search(tree->rhs, data, cmps);

    UNREACHABLE("bst_search");
}

void bst_search_symbol(bst_tree_t *tree, char symbol, size_t *cnt)
{
    if (!tree)
        return;

    if (tree->data[0] == symbol)
    {
        // printf("%s\n", tree->data);
        tree->is_repeated = true;
        if (cnt)
            *cnt += 1;

        bst_search_symbol(tree->lhs, symbol, cnt);
        bst_search_symbol(tree->rhs, symbol, cnt);
    }
    else if (tree->data[0] < symbol)
        bst_search_symbol(tree->rhs, symbol, cnt);
    else
        bst_search_symbol(tree->lhs, symbol, cnt);
}

static void to_dot(bst_tree_t *tree, void *fp)
{
    static int null_cnt = 0;

    if (tree->is_repeated)
        fprintf(fp, "  %s [color=\"green\"];\n", tree->data);

    if (tree->lhs)
        fprintf(fp, "  %s -> %s;\n", tree->data, tree->lhs->data);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->data, null_cnt);
        fprintf(fp, "  null_%d [shape=\"point\", color=\"red\"];\n", null_cnt);
        null_cnt++;
    }

    if (tree->rhs)
        fprintf(fp, "  %s -> %s;\n", tree->data, tree->rhs->data);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->data, null_cnt);
        fprintf(fp, "  null_%d [shape=\"point\", color=\"red\"];\n", null_cnt);
        null_cnt++;
    }
}

void bst_to_graphviz(FILE *fp, const char *tree_name, bst_tree_t *t)
{
    fprintf(fp, "digraph %s {\n", tree_name);
    bst_apply_pre(t, to_dot, fp);
    fprintf(fp, "}\n");
}

void bst_repeat_reset(bst_tree_t *tree)
{
    if (!tree)
        return;

    tree->is_repeated = false;
    bst_repeat_reset(tree->lhs);
    bst_repeat_reset(tree->rhs);
}

static int open_img(const char *img)
{
    pid_t pid = fork();
    if (pid == -1)
        return ERR_FORK;

    if (pid == 0)
    {
        int stdout_file = open("/dev/null", O_RDWR);
        if (dup2(stdout_file, STDERR_FILENO) == -1) // redirect fork'ed process stderr to /dev/null
            goto err;

        //     |> exec_name
        //     |       |> argv      |> it's necessary
        execlp("open", "open", img, NULL);

        err:
        close(stdout_file);

        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    else
    {
        int ret_code;
        waitpid(pid, &ret_code, 0);
        if (WEXITSTATUS(ret_code) != 0)
            return ERR_FORK;
    }
    return ERR_OK;
}

int bst_save_tmp_open(bst_tree_t *t)
{
    const char *gp = "temp.gp";
    const char *img = "tmp.png";

    FILE *fp = fopen(gp, "w");
    if (!fp)
        return ERR_IO;

    bst_to_graphviz(fp, "tree", t);

    fclose(fp);

    pid_t pid = fork();
    if (pid == -1)
        return ERR_FORK;

    if (pid == 0)
    {
        execlp("dot", "dot", "-Tpng", gp, "-o", img, NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    else
    {
        int ret_code;
        waitpid(pid, &ret_code, 0);
        if (WEXITSTATUS(ret_code) != 0)
            return ERR_FORK;
    }
    return open_img(img);
}

int bst_load_file(FILE *fp, bst_tree_t **tree)
{
    *tree = NULL;
    int rc = ERR_OK;

    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_IO;
    while (line)
    {
        rc = bst_insert_str(tree, line);
        free(line);
        if (rc != ERR_REPEAT && rc != ERR_OK) // TODO
            goto err;

        line = get_str(fp, NULL);
    }

    err:
    if (rc != ERR_REPEAT && rc != ERR_OK)
        bst_free(*tree);

    return rc;
}

int bst_load_file_ex(const char *filename, bst_tree_t **tree)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = bst_load_file(fp, tree);

    fclose(fp);
    return rc;
}

int bst_remove_nodes_starting_with(bst_tree_t **tree, char c)
{
    if (*tree == NULL)
        return ERR_OK;

    int rc;
    rc = bst_remove_nodes_starting_with(&(*tree)->lhs, c);
    if (rc == ERR_OK)
        rc = bst_remove_nodes_starting_with(&(*tree)->rhs, c);

    if (rc == ERR_OK && (*tree)->data[0] == c)
        rc = bst_remove_str(tree, (*tree)->data);

    return rc;
}

static void calc_depth_sum(bst_tree_t *root, int current_depth, int *total_depth, int *total_nodes)
{
    if (root == NULL)
        return;

    *total_depth += current_depth;
    (*total_nodes)++;

    calc_depth_sum(root->lhs, current_depth + 1, total_depth, total_nodes);
    calc_depth_sum(root->rhs, current_depth + 1, total_depth, total_nodes);
}

float bst_calc_avg_cmp(bst_tree_t *root)
{
    if (root == NULL)
        return 0.0f;

    int total_depth = 0;
    int total_nodes = 0;

    calc_depth_sum(root, 1, &total_depth, &total_nodes);

    return (float)total_depth / total_nodes;
}

void bst_calc_ram_usage(bst_tree_t *tree, size_t *bytes)
{
    if (tree == NULL)
        return;

    (*bytes) += sizeof(bst_tree_t);
    bst_calc_ram_usage(tree->lhs, bytes);
    bst_calc_ram_usage(tree->rhs, bytes);
}
