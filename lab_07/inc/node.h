#ifndef NODE_H__
#define NODE_H__

typedef struct node avl_tree_t;

struct node
{
    int value;
    avl_tree_t *next;
};

avl_tree_t *avl_node_create(void);

#endif // NODE_H__
