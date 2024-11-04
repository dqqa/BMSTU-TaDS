#include "errors.h"
#include "menu.h"
#include "queue_arr.h"
#include "queue_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Forward declarations */
int test_queue_list(void);
int test_queue_arr(void);

void print_guide(void)
{
    printf("Цель работы: отработка навыков работы с типом данных \"очередь\",\n"
           "представленном в виде массива и односвязного списка.\n");
}

int main(void)
{
    srand(time(NULL));
    print_guide();
    
    bool need_exit = false;
    int rc = ERR_OK;
    while (!need_exit)
    {
        main_menu_op_t op = get_main_menu_op();
        if (op == MAIN_MENU_OP_EOF || op == MAIN_MENU_OP_EXIT)
        {
            need_exit = true;
        }
        else if (op == MAIN_MENU_OP_LIST_QUEUE_TEST)
        {
            rc = test_queue_list();
        }
        else if (op == MAIN_MENU_OP_ARR_QUEUE_TEST)
        {
            rc = test_queue_arr();
        }
        else if (op == MAIN_MENU_OP_SIMULATE)
        {
            // TODO: SIMULATION
        }
        else if (op == MAIN_MENU_OP_MEASURE)
        {
            // TODO: MEASUREMENTS
        }
        else if (op == MAIN_MENU_OP_UNKNOWN)
        {
            rc = ERR_BAD_NUM;
        }

        out:
        if (rc != ERR_OK)
        {
            printf("Ошибка!");
            rc = ERR_OK;
        }
    }

    return rc;
}
