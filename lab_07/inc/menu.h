#ifndef MENU_H__
#define MENU_H__

typedef enum
{
    MAIN_EXIT,
    MAIN_CHECK_BST,
    MAIN_CHECK_AVL,
    MAIN_CHECK_HASH_OPEN,
    MAIN_CHECK_HASH_CLOSED,
    // MAIN_PERF_CMP,
    MAIN_EDIT_PARAM,
    MAIN_COUNT,
    MAIN_UNKNOWN
} main_menu_action_t;

typedef enum
{
    TREE_ACT_EXIT,
    TREE_ACT_LOAD_FILE,
    TREE_ACT_DELETE_WORDS,
    TREE_ACT_SHOW,
    TREE_ACT_SEARCH_WORD,
    TREE_ACT_SEARCH_STARTING,
    TREE_ACT_ADD,
    TREE_ACT_AVG_CMP,
    TREE_ACT_CALC_SIZE,
    TREE_ACT_COUNT,
    TREE_ACT_UNKNOWN
} tree_action_t;

typedef enum
{
    HT_ACT_EXIT,
    HT_ACT_LOAD_FILE,
    HT_ACT_SHOW,
    HT_ACT_SEARCH,
    HT_ACT_REMOVE,
    HT_ACT_ADD,
    HT_ACT_AVG_CMP,
    HT_ACT_CALC_SIZE,
    HT_ACT_COUNT,
    HT_ACT_UNKNOWN
} ht_action_t;

main_menu_action_t get_main_menu_act(void);
tree_action_t get_tree_menu_act(void);
ht_action_t get_ht_menu_act(void);

int check_bst(void);
int check_avl(void);
int check_ht_open(void);
int check_ht_closed(void);
int perf_cmp(void);

#endif // MENU_H__
