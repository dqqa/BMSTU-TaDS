#define _GNU_SOURCE

#include "tree.h"
#include "errors.h"
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

int tree_create(tree_t **t, const char *data)
{
    int rc = ERR_OK;
    *t = NULL;

    tree_t *tree = malloc(sizeof(*tree));
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

void tree_free(tree_t *tree)
{
    if (!tree)
        return;

    tree_free(tree->lhs);
    tree_free(tree->rhs);

    free(tree->data);
    free(tree);
}

void tree_apply_pre(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    apply_fn(tree, arg);
    tree_apply_pre(tree->lhs, apply_fn, arg);
    tree_apply_pre(tree->rhs, apply_fn, arg);
}

void tree_apply_in(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    tree_apply_in(tree->lhs, apply_fn, arg);
    apply_fn(tree, arg);
    tree_apply_in(tree->rhs, apply_fn, arg);
}

void tree_apply_post(tree_t *tree, tree_apply_fn_t apply_fn, void *arg)
{
    if (!tree)
        return;

    tree_apply_post(tree->lhs, apply_fn, arg);
    tree_apply_post(tree->rhs, apply_fn, arg);
    apply_fn(tree, arg);
}

tree_t *tree_insert_node(tree_t *tree, tree_t *src, int *err)
{
    if (!tree)
        return src;

    *err = ERR_OK;

    int cmp_res = strcmp(tree->data, src->data);
    if (cmp_res == 0)
        *err = ERR_REPEAT; // assert(0 && "Yet there is no support of repeating nodes!");
    else if (cmp_res < 0)
        tree->rhs = tree_insert_node(tree->rhs, src, err);
    else
        tree->lhs = tree_insert_node(tree->lhs, src, err);

    return tree;
}

int tree_insert_str(tree_t **tree, const char *src)
{
    tree_t *subtree;
    int rc = tree_create(&subtree, src);
    if (rc != ERR_OK)
        return rc;

    tree_t *newtree = tree_insert_node(*tree, subtree, &rc);
    if (rc != ERR_OK)
        goto err;

    *tree = newtree;

    err:
    if (rc != ERR_OK)
        tree_free(subtree);

    return rc;
}

static tree_t *tree_min_node(tree_t *node)
{
    tree_t *current = node;
    while (current && current->lhs != NULL)
    {
        current = current->lhs;
    }
    return current;
}

int tree_remove(tree_t **tree, tree_t *what)
{
    if (*tree == NULL)
        return ERR_PARAM;

    int cmp = strcmp((*tree)->data, what->data);
    if (cmp > 0)
        return tree_remove(&(*tree)->lhs, what);
    else if (cmp < 0)
        return tree_remove(&(*tree)->rhs, what);
    else
    {
        if ((*tree)->lhs == NULL)
        {
            tree_t *temp = (*tree)->rhs;
            free((*tree)->data);
            free(*tree);
            *tree = temp;
        }
        else if ((*tree)->rhs == NULL)
        {
            tree_t *temp = (*tree)->lhs;
            free((*tree)->data);
            free(*tree);
            *tree = temp;
        }
        else
        {
            tree_t *temp = tree_min_node((*tree)->rhs);
            free((*tree)->data);
            (*tree)->data = strdup(temp->data);
            (*tree)->is_repeated = temp->is_repeated;
            tree_remove(&(*tree)->rhs, temp);
        }
        return ERR_OK;
    }
}

int tree_remove_str(tree_t **tree, const char *what)
{
    tree_t *node_to_remove = tree_search(*tree, what);
    if (!node_to_remove)
        return ERR_NOT_FOUND;

    return tree_remove(tree, node_to_remove);
}

tree_t *tree_search(tree_t *tree, const char *data)
{
    if (!tree)
        return NULL;

    int cmp_res = strcmp(data, tree->data);
    if (cmp_res == 0)
        return tree;
    else if (cmp_res < 0)
        return tree_search(tree->lhs, data);
    else
        return tree_search(tree->rhs, data);

    UNREACHABLE("tree_search");
}

void tree_search_symbol(tree_t *tree, char symbol, size_t *cnt)
{
    if (!tree)
        return;

    if (tree->data[0] == symbol)
    {
        // printf("%s\n", tree->data);
        tree->is_repeated = true;
        if (cnt)
            *cnt += 1;

        tree_search_symbol(tree->lhs, symbol, cnt);
        tree_search_symbol(tree->rhs, symbol, cnt);
    }
    else if (tree->data[0] < symbol)
        tree_search_symbol(tree->rhs, symbol, cnt);
    else
        tree_search_symbol(tree->lhs, symbol, cnt);
}

static void to_dot(tree_t *tree, void *fp)
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

void tree_to_graphviz(FILE *fp, const char *tree_name, tree_t *t)
{
    fprintf(fp, "digraph %s {\n", tree_name);
    tree_apply_pre(t, to_dot, fp);
    fprintf(fp, "}\n");
}

void tree_repeat_reset(tree_t *tree)
{
    if (!tree)
        return;

    tree->is_repeated = false;
    tree_repeat_reset(tree->lhs);
    tree_repeat_reset(tree->rhs);
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

int tree_save_tmp_open(tree_t *t)
{
    const char *gp = "temp.gp";
    const char *img = "tmp.png";

    FILE *fp = fopen(gp, "w");
    if (!fp)
        return ERR_IO;

    tree_to_graphviz(fp, "tree", t);

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
