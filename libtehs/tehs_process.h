//
// Created by zyx on 2020/10/14.
//

#ifndef _TEHS_PROCESS_H_INCLUDED_
#define _TEHS_PROCESS_H_INCLUDED_



typedef void (*tehs_spawn_proc_pt)();

typedef struct{
    pid_t pid;
    int status;
    int channel[2];
    char *name;
    tehs_spawn_proc_pt proc;
}tehs_process_t;

typedef struct {
    int signo;
    char *signame;
    char *name;
    void (*handler)(int signo);
}tehs_signal_t;

int tehs_spawn_process();

extern int tehs_pid;
extern int tehs_parent;
extern int tehs_last_process;
extern tehs_process_t tehs_processes[1024];

#endif