#include "avl_tree.h"
#include "str.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Создает АВЛ-дерево
 *
 * @return node_t* Новый узел
 */
avl_tree_t *avl_create(const char *data)
{
    int rc = ERR_OK;
    avl_tree_t *tree = malloc(sizeof(*tree));
    if (!tree)
        return NULL;

    memset(tree, 0, sizeof(*tree));
    tree->height = 1;

    tree->key = strdup(data);
    if (!tree->key)
    {
        rc = ERR_ALLOC;
        goto err;
    }

    err:
    if (rc != ERR_OK)
    {
        free(tree->key);
        free(tree);

        tree = NULL;
    }

    return tree;
}

/**
 * @brief Освобождает память из-под узла
 *
 * @param node Двойной указатель на узел
 */
void avl_node_free(avl_tree_t **node)
{
    free((*node)->key);
    free(*node);

    *node = NULL;
}

/**
 * @brief Вставляет узел в АВЛ-дерево
 *
 * @param head Двойной указатель на голову
 * @param new_node Узел, который требуется вставить
 * @return int Код оишбки
 */
int avl_insert_node(avl_tree_t **head, avl_tree_t *new_node)
{
    if (head == NULL || new_node == NULL)
        return ERR_PARAM;

    if (*head == NULL)
    {
        *head = new_node;
        return ERR_OK;
    }

    int cmp = strcmp((*head)->key, new_node->key);
    if (cmp == 0)
        return ERR_REPEAT;

    int rc = ERR_OK;
    if (cmp > 0)
        rc = avl_insert_node(&(*head)->lhs, new_node);
    else
        rc = avl_insert_node(&(*head)->rhs, new_node);

    // if (rc == ERR_OK)
    //     *head = avl_node_balance(*head);

    avl_fix_height(*head);

    int balance = avl_calc_balance_factor(*head);
    if (balance > 1)
    {
        int left_cmp = strcmp(new_node->key, (*head)->lhs->key);
        if (left_cmp < 0)
        {
            *head = avl_rotate_right(*head);
            return ERR_OK;
        }
        else if (left_cmp > 0)
        {
            (*head)->lhs = avl_rotate_left((*head)->lhs);
            *head = avl_rotate_right(*head);
            return ERR_OK;
        }
    }

    if (balance < -1)
    {
        int right_cmp = strcmp(new_node->key, (*head)->rhs->key);
        if (right_cmp > 0)
        {
            *head = avl_rotate_left(*head);
            return ERR_OK;
        }
        else if (right_cmp < 0)
        {
            (*head)->rhs = avl_rotate_right((*head)->rhs);
            *head = avl_rotate_left(*head);
            return ERR_OK;
        }
    }

    return rc;
}

int avl_insert_str(avl_tree_t **head, const char *key)
{
    if (head == NULL || key == NULL)
        return ERR_PARAM;

    if (strlen(key) == 0)
        return ERR_PARAM;

    avl_tree_t *newnode = avl_create(key);
    if (newnode == NULL)
        return ERR_ALLOC;

    int rc = avl_insert_node(head, newnode);

    if (rc != ERR_OK)
        avl_node_free(&newnode);

    return rc;
}

/**
 * @brief Освобождает память из-под АВЛ-дерева
 *
 * @param head Двойной указатель на голову
 */
void avl_free(avl_tree_t **head)
{
    if (head == NULL || *head == NULL)
        return;

    avl_free(&(*head)->lhs);
    avl_free(&(*head)->rhs);

    avl_node_free(head);
}

/**
 * @brief Ищет узел в АВЛ-дереве по ключу
 *
 * @param head Указатель на голову
 * @param key Ключ
 * @return const node_t* Указатель на искомый узел
 */
avl_tree_t *avl_search(avl_tree_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    int cmp = strcmp(head->key, key);
    if (cmp == 0)
    {
        head->is_repeated = true;
        return head;
    }
    else if (cmp > 0)
        return avl_search(head->lhs, key);
    else
        return avl_search(head->rhs, key);
}

/**
 * @brief Удаляет узел с минимальным ключом в АВЛ-дереве
 *
 * @param head Указтель на голову
 * @return node_t* Указатель на голову
 */
avl_tree_t *avl_remove_min(avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    if (head->lhs == NULL)
        return head->rhs;

    head->lhs = avl_remove_min(head->lhs);

    return avl_node_balance(head);
}

/**
 * @brief Удаляет узел АВЛ-дерева по ключу
 *
 * @param head Указатель на голову
 * @param key Ключ
 * @return node_t* Указатель на голову
 */
avl_tree_t *avl_remove(avl_tree_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    int cmp = strcmp(head->key, key);
    if (cmp > 0)
        head->lhs = avl_remove(head->lhs, key);
    else if (cmp < 0)
        head->rhs = avl_remove(head->rhs, key);
    else
    {
        avl_tree_t *lhs = head->lhs;
        avl_tree_t *rhs = head->rhs;

        free(head->key);
        free(head);
        if (rhs == NULL)
            return lhs;

        avl_tree_t *min = (avl_tree_t *)avl_find_min(rhs);
        // Сначала удалить минимальный элемент в правом поддереве, после чего
        // присвоить левое поддерево
        min->rhs = avl_remove_min(rhs);
        min->lhs = lhs;

        head = min;
    }

    return avl_node_balance(head);
}

/**
 * @brief Применяет callback-функцию к каждому узлу АВЛ-дерева префиксно
 *
 * @param head Указатель на голову
 * @param func Callback-функция
 * @param param Необязательный параметр для callback функции
 */
void avl_apply_pre(avl_tree_t *head, avl_apply_fn_t func, void *param)
{
    if (head == NULL)
        return;

    func(head, param);
    avl_apply_pre(head->lhs, func, param);
    avl_apply_pre(head->rhs, func, param);
}

/**
 * @brief Применяет callback-функцию к каждому узлу АВЛ-дерева инфиксно
 *
 * @param head Указатель на голову
 * @param func Callback-функция
 * @param param Необязательный параметр для callback функции
 */
void avl_apply_in(avl_tree_t *head, avl_apply_fn_t func, void *param)
{
    if (head == NULL)
        return;

    avl_apply_in(head->lhs, func, param);
    func(head, param);
    avl_apply_in(head->rhs, func, param);
}

/**
 * @brief Применяет callback-функцию к каждому узлу АВЛ-дерева постфиксно
 *
 * @param head Указатель на голову
 * @param func Callback-функция
 * @param param Необязательный параметр для callback функции
 */
void avl_apply_post(avl_tree_t *head, avl_apply_fn_t func, void *param)
{
    if (head == NULL)
        return;

    avl_apply_post(head->lhs, func, param);
    avl_apply_post(head->rhs, func, param);
    func(head, param);
}

/**
 * @brief Ищет узел с минимальным ключом
 *
 * @param head Указатель на голову
 * @return const node_t* Узел с минимальным ключом
 */
const avl_tree_t *avl_find_min(const avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    if (head->lhs != NULL)
        return avl_find_min(head->lhs);

    return head;
}

/**
 * @brief Ищет узел с максимальным ключом
 *
 * @param head Указатель на голову
 * @return const node_t* Узел с максимальным ключом
 */
const avl_tree_t *avl_find_max(const avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    if (head->rhs != NULL)
        return avl_find_max(head->rhs);

    return head;
}

/**
 * @brief Поворачивает поддерево влево
 *
 * @param head Указатель на голову
 * @return node_t* Указатель на голову
 */
avl_tree_t *avl_rotate_left(avl_tree_t *head)
{
    avl_tree_t *tmp = head->rhs;
    head->rhs = tmp->lhs;
    tmp->lhs = head;

    avl_fix_height(head);
    avl_fix_height(tmp);

    return tmp;
}

/**
 * @brief Поворачивает поддерево вправо
 *
 * @param head Указатель на голову
 * @return node_t* Указатель на голову
 */
avl_tree_t *avl_rotate_right(avl_tree_t *head)
{
    avl_tree_t *tmp = head->lhs;
    head->lhs = tmp->rhs;
    tmp->rhs = head;

    avl_fix_height(head);
    avl_fix_height(tmp);

    return tmp;
}

/**
 * @brief Восстанавливает корректное значение высоты узла
 *
 * Ограничение: высоты правого и левого поддерева должны быть также корректными
 *
 * @param head Указатель на узел
 */
void avl_fix_height(avl_tree_t *head)
{
    int lhs_height = avl_get_height(head->lhs);
    int rhs_height = avl_get_height(head->rhs);
    if (lhs_height > rhs_height)
        head->height = lhs_height + 1;
    else
        head->height = rhs_height + 1;
}

/**
 * @brief Балансирует АВЛ-дерево
 *
 * @param head Указатель на голову
 * @return node_t* Указатель на голову
 */
avl_tree_t *avl_node_balance(avl_tree_t *head)
{
    if (head == NULL)
        return NULL;

    avl_fix_height(head);

    int factor = avl_calc_balance_factor(head);
    if (factor > 1)
    {
        if (avl_calc_balance_factor(head->lhs) < 0)
            head->lhs = avl_rotate_left(head->lhs);
        return avl_rotate_right(head);
    }

    if (factor < -1)
    {
        if (avl_calc_balance_factor(head->rhs) > 0)
            head->rhs = avl_rotate_right(head->rhs);
        return avl_rotate_left(head);
    }

    return head;
}

/**
 * @brief Получает высоту узла АВЛ-дерева
 *
 * @param head Укзаатель на узел
 * @return int Высота узла
 */
int avl_get_height(const avl_tree_t *head)
{
    if (head != NULL)
        return head->height;

    return 0;
}

/**
 * @brief Считает сбалансированность для узла
 *
 * @param head Указатель на узел
 * @return int Сбалансированность узла
 */
int avl_calc_balance_factor(const avl_tree_t *head)
{
    if (head == NULL)
        return 0;

    return avl_get_height(head->lhs) - avl_get_height(head->rhs);
}

int avl_load_file(FILE *fp, avl_tree_t **tree)
{
    *tree = NULL;
    int rc = ERR_OK;

    char *line = get_str(fp, NULL);
    if (!line)
        return ERR_IO;
    while (line)
    {
        rc = avl_insert_str(tree, line);
        free(line);
        if (rc != ERR_REPEAT && rc != ERR_OK) // TODO
            goto err;

        line = get_str(fp, NULL);
    }

    err:
    if (rc != ERR_REPEAT && rc != ERR_OK)
        avl_free(tree);

    return rc;
}

int avl_load_file_ex(const char *filename, avl_tree_t **tree)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return ERR_IO;

    int rc = avl_load_file(fp, tree);

    fclose(fp);
    return rc;
}

int avl_remove_nodes_starting_with(avl_tree_t **tree, char c)
{
    if (*tree == NULL)
        return ERR_OK;

    int rc;
    rc = avl_remove_nodes_starting_with(&(*tree)->lhs, c);
    if (rc == ERR_OK)
        rc = avl_remove_nodes_starting_with(&(*tree)->rhs, c);

    if (rc == ERR_OK && (*tree)->key[0] == c)
        *tree = avl_remove(*tree, (*tree)->key);

    *tree = avl_node_balance(*tree);

    return rc;
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

static void to_dot(avl_tree_t *tree, void *fp)
{
    static int null_cnt = 0;

    if (tree->is_repeated)
        fprintf(fp, "  %s [color=\"green\"];\n", tree->key);

    if (tree->lhs)
        fprintf(fp, "  %s -> %s;\n", tree->key, tree->lhs->key);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->key, null_cnt);
        fprintf(fp, "  null_%d [shape=\"point\", color=\"red\"];\n", null_cnt);
        null_cnt++;
    }

    if (tree->rhs)
        fprintf(fp, "  %s -> %s;\n", tree->key, tree->rhs->key);
    else
    {
        fprintf(fp, "  %s -> null_%d;\n", tree->key, null_cnt);
        fprintf(fp, "  null_%d [shape=\"point\", color=\"red\"];\n", null_cnt);
        null_cnt++;
    }
}

void avl_to_graphviz(FILE *fp, const char *tree_name, avl_tree_t *t)
{
    fprintf(fp, "digraph %s {\n", tree_name);
    avl_apply_pre(t, to_dot, fp);
    fprintf(fp, "}\n");
}

int avl_save_tmp_open(avl_tree_t *t)
{
    const char *gp = "temp.gp";
    const char *img = "tmp.png";

    FILE *fp = fopen(gp, "w");
    if (!fp)
        return ERR_IO;

    avl_to_graphviz(fp, "tree", t);

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

void avl_repeat_reset(avl_tree_t *tree)
{
    if (!tree)
        return;

    tree->is_repeated = false;
    avl_repeat_reset(tree->lhs);
    avl_repeat_reset(tree->rhs);
}

void avl_search_symbol(avl_tree_t *tree, char symbol, size_t *cnt)
{
    if (!tree)
        return;

    if (tree->key[0] == symbol)
    {
        // printf("%s\n", tree->data);
        tree->is_repeated = true;
        if (cnt)
            *cnt += 1;

        avl_search_symbol(tree->lhs, symbol, cnt);
        avl_search_symbol(tree->rhs, symbol, cnt);
    }
    else if (tree->key[0] < symbol)
        avl_search_symbol(tree->rhs, symbol, cnt);
    else
        avl_search_symbol(tree->lhs, symbol, cnt);
}

static void calc_depth_sum(avl_tree_t *root, int current_depth, int *total_depth, int *total_nodes)
{
    if (root == NULL)
        return;

    *total_depth += current_depth;
    (*total_nodes)++;

    calc_depth_sum(root->lhs, current_depth + 1, total_depth, total_nodes);
    calc_depth_sum(root->rhs, current_depth + 1, total_depth, total_nodes);
}

float avl_calc_avg_cmp(avl_tree_t *root)
{
    if (root == NULL)
        return 0.0f;

    int total_depth = 0;
    int total_nodes = 0;

    calc_depth_sum(root, 1, &total_depth, &total_nodes);

    return (float)total_depth / total_nodes;
}
