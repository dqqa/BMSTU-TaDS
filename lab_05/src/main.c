#include "errors.h"
#include "queue_arr.h"
#include "queue_list.h"
#include <stdio.h>

void apply_fn(node_t *node, void *fmt)
{
    printf(fmt, *(int*)node->data);
}

void apply_fn_arr(data_t *node, void *fmt)
{
    printf(fmt, *node);
}

int main2(void)
{
    queue_list_t q = { 0 };
    queue_list_create(&q);
    queue_list_push(&q, &(int){ 1 }, 4);
    queue_list_push(&q, &(int){ 2 }, 4);
    queue_list_push(&q, &(int){ 3 }, 4);
    queue_list_push(&q, &(int){ 4 }, 4);

    int rc = ERR_OK;
    int el;
    queue_list_apply(&q, apply_fn, "%d\n");
    // while ((rc=queue_list_pop(&q, &el, sizeof(el))) == ERR_OK)
    // {
    //     printf("%d ", el);
    // }
    printf("\n");
    queue_list_free(&q);

    return 0;
}

int main(void)
{
    queue_arr_t q = { 0 };
    queue_arr_create(&q);
    queue_arr_push(&q, &(int){ 1 });
    queue_arr_push(&q, &(int){ 2 });
    queue_arr_push(&q, &(int){ 3 });
    queue_arr_push(&q, &(int){ 4 });

    int rc = ERR_OK;
    int el;
    queue_arr_apply(&q, apply_fn_arr, "%d\n");
    // while ((rc=queue_arr_pop(&q, &el, sizeof(el))) == ERR_OK)
    // {
    //     printf("%d ", el);
    // }
    printf("\n");

    return 0;
}
