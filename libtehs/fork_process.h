//
// Created by zyx on 2020/10/14.
//

#ifndef FORK_PROCESS_
#define FORK_PROCESS_


typedef void(*active_t)();

typedef struct{
    pid_t pid;
    int status;
    int channel[2];
}tehs_proccess_t;

typedef struct {
    int signo;
    char *signame;
    char *name;
    void (*handler)(int signo);
}tehs_signal_t;

static void tehs_signal_handler(int signo);

void tehs_master_process_cycle();

#endif