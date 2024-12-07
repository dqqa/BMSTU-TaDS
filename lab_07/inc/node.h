#ifndef NODE_H__
#define NODE_H__

typedef struct node avl_node_t;

struct node
{
    int value;
    avl_node_t *next;
};

avl_node_t *avl_node_create(void);

#endif // NODE_H__
