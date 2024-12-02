#include "avl_tree.h"
#include "errors.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Создает узел АВЛ-дерева
 *
 * @return node_t* Новый узел
 */
node_t *node_create(void)
{
    node_t *node = malloc(sizeof(*node));
    if (node)
    {
        memset(node, 0, sizeof(*node));
    }

    return node;
}

/**
 * @brief Вставляет узел в АВЛ-дерево
 *
 * @param head Двойной указатель на голову
 * @param new_node Узел, который требуется вставить
 * @return assoc_array_error_t Код оишбки
 */
int avl_insert(node_t **head, node_t *new_node)
{
    assert(head);
    assert(new_node);

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
        rc = avl_insert(&(*head)->rhs, new_node);
    else
        rc = avl_insert(&(*head)->lhs, new_node);

    if (rc == ERR_OK)
        *head = avl_node_balance(*head);

    return rc;
}

/**
 * @brief Освобождает память из-под АВЛ-дерева
 *
 * @param head Двойной указатель на голову
 */
void avl_free(node_t **head)
{
    assert(head);
    if (*head == NULL)
        return;

    avl_free(&(*head)->lhs);
    avl_free(&(*head)->rhs);

    free(*head);
    *head = NULL;
}

/**
 * @brief Ищет узел в АВЛ-дереве по ключу
 *
 * @param head Указатель на голову
 * @param key Ключ
 * @return const node_t* Указатель на искомый узел
 */
const node_t *avl_search(const node_t *head, const char *key)
{
    if (head == NULL)
        return NULL;

    int cmp = strcmp(head->key, key);
    if (cmp == 0)
        return head;
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
node_t *avl_remove_min(node_t *head)
{
    assert(head);
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
node_t *avl_remove(node_t *head, const char *key)
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
        node_t *lhs = head->lhs;
        node_t *rhs = head->rhs;

        free(head);
        if (rhs == NULL)
            return lhs;

        node_t *min = (node_t *)avl_find_min(rhs);
        min->lhs = lhs;
        min->rhs = avl_remove_min(rhs);

        head = min;
    }

    return avl_node_balance(head);
}

/**
 * @brief Применяет callback-функцию к каждому узлу АВЛ-дерева
 *
 * @param head Указатель на голову
 * @param func Callback-функция
 * @param param Необязательный параметр для callback функции
 */
void avl_apply(node_t *head, avl_apply_fn_t func, void *param)
{
    if (head == NULL)
        return;

    func(head->key, &head->value, param);
    avl_apply(head->lhs, func, param);
    avl_apply(head->rhs, func, param);
}

/**
 * @brief Ищет узел с минимальным ключом
 *
 * @param head Указатель на голову
 * @return const node_t* Узел с минимальным ключом
 */
const node_t *avl_find_min(const node_t *head)
{
    assert(head);
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
const node_t *avl_find_max(const node_t *head)
{
    assert(head);
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
node_t *avl_rotate_left(node_t *head)
{
    node_t *tmp = head->rhs;
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
node_t *avl_rotate_right(node_t *head)
{
    node_t *tmp = head->lhs;
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
void avl_fix_height(node_t *head)
{
    char lhs_height = avl_get_height(head->lhs);
    char rhs_height = avl_get_height(head->rhs);
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
node_t *avl_node_balance(node_t *head)
{
    avl_fix_height(head);

    int factor = avl_calc_balance_factor(head);
    if (factor == 2)
    {
        if (avl_calc_balance_factor(head->rhs) < 0)
            head->rhs = avl_rotate_right(head->rhs);
        return avl_rotate_left(head);
    }

    if (factor == -2)
    {
        if (avl_calc_balance_factor(head->lhs) > 0)
            head->lhs = avl_rotate_left(head->lhs);
        return avl_rotate_right(head);
    }

    return head;
}

/**
 * @brief Получает высоту узла АВЛ-дерева
 *
 * @param head Укзаатель на узел
 * @return char Высота узла
 */
char avl_get_height(const node_t *head)
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
int avl_calc_balance_factor(const node_t *head)
{
    assert(head);
    return avl_get_height(head->rhs) - avl_get_height(head->lhs);
}
