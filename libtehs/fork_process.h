//
// Created by zyx on 2020/10/14.
//

#ifndef PRCESSANDTHREAD_SRC_FORKPROCESS_H_
#define PRCESSANDTHREAD_SRC_FORKPROCESS_H_


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

void fork_new_proc(active_t childFunc, active_t parentFunc);

void fork_new_proc(active_t childFunc);
void save_child_id(string home,__pid_t pid,string desc);

#endif