//
// Created by zyx on 2020/10/14.
//

#include <zconf.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include "fork_process.h"

using namespace std;


void fork_new_proc(active_t childFunc){
  pid_t pid = fork();

  if(pid == 0){
    int child_PID = getpid();

    printf("child id: %d\n",child_PID);

    childFunc();

    printf("stop child process %d...\n",child_PID);

    string command = "touch /home/zyx/workspace/tehs/tmp/"+to_string(getpid())+".lock";

    system(command.c_str());

  }  else if (pid < 0){
    printf("FORK FAILED\n");

    _exit(1);
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
