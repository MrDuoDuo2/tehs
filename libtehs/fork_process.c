//
// Created by zyx on 2020/10/14.
//

#include <zconf.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <tclDecls.h>
#include "fork_process.h"

using namespace std;


void fork_new_proc(active_t childFunc){
    tehs_proccess_t tehsProccess[1024];
    if(socketpair(AF_UNIX,SOCK_STREAM,0,tehsProccess[1].channel)==-1){
        pid_t pid = fork();

        if(pid == 0){
            int child_PID = getpid();

            printf("child id: %d\n",child_PID);

            childFunc();

            printf("stop child process %d...\n",child_PID);

        }  else if (pid < 0){
            printf("FORK FAILED\n");

            _exit(1);
        }
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
