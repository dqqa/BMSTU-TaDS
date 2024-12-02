#ifndef NODE_H__
#define NODE_H__

typedef struct node node_t;

struct node
{
    const char *key;
    int value;

    node_t *next;
};

node_t *node_create(void);

#endif // NODE_H__
