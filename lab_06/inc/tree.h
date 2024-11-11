#ifndef TREE_H__
#define TREE_H__

typedef char *data_t;

typedef struct __tree_t
{
    data_t data;
    struct __tree_t *lhs, *rhs;
} tree_t;

typedef void (*tree_apply_fn_t)(tree_t *subtree, void *arg);

int tree_create(tree_t **t, const char *data);
void tree_free(tree_t *tree);

void tree_apply_pre(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);
void tree_apply_in(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);
void tree_apply_post(tree_t *tree, tree_apply_fn_t apply_fn, void *arg);

tree_t *tree_insert_node(tree_t *tree, tree_t *src, int *err);
int tree_insert_str(tree_t **tree, const char *src);

void tree_remove(tree_t *tree, tree_t *what);
tree_t *tree_search(tree_t *tree, const char *data);

#endif // TREE_H__
