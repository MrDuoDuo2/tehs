//
// Created by zyx on 2021/11/14.
//

#include "tehs_process_cycle.h"
#include "tehs_process.h"
#include <signal.h>
#include <stddef.h>

static void tehs_start_work_process_cycle(int num);
uintptr_t tehs_process;

void
tehs_master_process_cycle(){

  sigset_t set;

  sigemptyset(&set);
  sigaddset(&set,SIGCHLD);//子进程结束，父进程会接收到此信号
  sigaddset(&set,SIGALRM);//时钟信号
  sigaddset(&set,SIGIO);
  sigaddset(&set,SIGINT);

  if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {

  }

  tehs_start_work_process_cycle(2);



}

static void
tehs_start_work_process_cycle(int num){
    int i;
    for (i = 0; i < num; i++) {
        tehs_spawn_process();
    }
}