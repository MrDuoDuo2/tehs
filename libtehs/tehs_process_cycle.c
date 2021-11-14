//
// Created by zyx on 2021/11/14.
//

#include "tehs_process_cycle.h"
#include "tehs_process.h"

static void tehs_start_work_process_cycle();

void
tehs_master_process_cycle(){


    tehs_start_work_process_cycle();

}

static void
tehs_start_work_process_cycle(int num){
    int i;
    for (i = 0; i < num; i++) {
        tehs_spawn_process();
    }
}