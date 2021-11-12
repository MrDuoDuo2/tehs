//
// Created by zyx on 2020/10/14.
//

#include <zconf.h>
#include <sys/socket.h>
#include <bits/sigaction.h>
#include "fork_process.h"

tehs_signal_t signals[]={
        {1,"SIGHUP","reload",tehs_signal_handler}
};

int
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


}


void fork_new_proc(active_t childFunc){
//    tehs_proccess_t tehsProccess[1024];
//    if(socketpair(AF_UNIX,SOCK_STREAM,0,tehsProccess[1].channel)==-1){
//        pid_t pid = fork();
//
//        if(pid == 0){
//            int child_PID = getpid();
//
//            printf("child id: %d\n",child_PID);
//
//            childFunc();
//
//            printf("stop child process %d...\n",child_PID);
//
//        }  else if (pid < 0){
//            printf("FORK FAILED\n");
//
//            _exit(1);
//        }
//    }
    for(;;){
//        sigsuspend(&set);
    }

}

void fork_new_proc(active_t childFunc,active_t parentFunc) {
    pid_t pid = fork();

    if(pid == 0){
      int child_PID = getpid();

      printf("child id: %d\n",child_PID);

      childFunc();

      printf("stop child process...\n");
    } else if (pid > 0) {
      printf("this is parent process...%d\n",getpid());

      parentFunc();
    }  else {
      printf("FORK FAILED");

      _exit(1);
    }
}
