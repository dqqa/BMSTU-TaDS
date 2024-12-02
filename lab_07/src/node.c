#include "node.h"
#include <stdlib.h>

/**
 * @brief Создает узел односвязного списка
 * 
 * @return node_t* Указатель на узел
 */
node_t *node_create(void)
{
    node_t *new_node = malloc(sizeof(*new_node));
    if (new_node)
    {
        new_node->key = NULL;
        new_node->value = 0;
        new_node->next = NULL;
    }

    return new_node;
}
