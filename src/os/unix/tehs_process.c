//
// Created by zyx on 2020/10/14.
//

#include <tehs_core.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <zconf.h>

static int tehs_init_signals();
static void tehs_signal_handler(int signo);

tehs_signal_t signals[]={
    {1,"SIGHUP","reload",tehs_signal_handler},
    { 0, NULL, "", NULL }
};


//创建新进程
int tehs_spawn_process(tehs_spawn_proc_pt proc,char *name){
    int index;

    for (index = 0; index < tehs_last_process; index++) {
        if(tehs_processes[index].pid == -1){
            break;
        }
    }

    if(socketpair(AF_UNIX,SOCK_STREAM,0,tehs_processes[index].channel)==-1){
        printf("socket start faild!");
    }

    pid_t pid = fork();

    switch (pid) {
        case -1:
            printf("start new process faild!");
            return 0;
        case 0:
            tehs_parent = tehs_pid;
            tehs_pid = getpid();
            proc();
            break;
        default:
            break;
    }

    tehs_processes[index].pid = pid;
    tehs_processes[index].proc = proc;
    tehs_processes[index].name = name;

    return pid;
}

//初始化信号
static int
tehs_init_signals(){
    tehs_signal_t *sig;
    struct sigaction sa;

    for (sig = signals; sig->signo != 0 ; sig++) {

        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1){
            return 0;
        }
    }
    return 1;
}

static void
tehs_signal_handler(int signo) {
    tehs_signal_t *sig;

    for (sig = signals; sig->signo != 0; sig++) {
        if (sig->signo == signo) {
            break;
        }
    }

    if (tehs_process){

    }

}