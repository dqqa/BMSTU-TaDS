#include "node.h"
#include <stdlib.h>

/**
 * @brief Создает узел односвязного списка
 * 
 * @return node_t* Указатель на узел
 */
avl_tree_t *avl_node_create(void)
{
    avl_tree_t *new_node = malloc(sizeof(*new_node));
    if (new_node)
    {
        new_node->key = NULL;
        new_node->value = 0;
        new_node->next = NULL;
    }

    return new_node;
}
