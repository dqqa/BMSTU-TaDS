#ifndef TREE_H__
#define TREE_H__

typedef char *data_t;

typedef struct __tree_t
{
    data_t data;
    struct __tree_t *lhs, *rhs;
} tree_t;

typedef void (*tree_apply_fn_t)(tree_t *subtree, void *arg);

int tree_create(tree_t **t);
void tree_free(tree_t *tree);

int tree_apply(const tree_t *tree, tree_apply_fn_t apply_fn, void *arg);
int tree_insert(tree_t **tree, const data_t *src);
int tree_remove(tree_t *tree, data_t **dst);
int tree_search(const tree_t *tree, const data_t *data);

#endif // TREE_H__
