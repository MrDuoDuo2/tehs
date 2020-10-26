//
// Created by zyx on 2020/10/14.
//

#include <zconf.h>
#include <cstdio>
#include <cstdlib>
#include "forkProcess.h"

using namespace std;

void saveChildID(__pid_t pid) {
    FILE *file;
    char buffer[10];
    char filename[40];
    char homeSrc[50] = "/home/zyx/workspace/tehs";

    sprintf(buffer, "%d", pid);
    sprintf(filename, "%s/config/proc/childProcess%d",homeSrc, pid);

    //保存文件
    file = fopen(filename,"w");
    fputs(buffer, file);
    fclose(file);
}

forkProcess::forkProcess(active_t childFunc){
  pid_t pid = fork();

  if(pid == 0){
    int child_PID = getpid();

    printf("child id: %d\n",child_PID);

    saveChildID(child_PID);

    childFunc();

    printf("stop child process %d...\n",child_PID);
  }  else if (pid < 0){
    printf("FORK FAILED\n");

    _exit(1);
  }
}

forkProcess::forkProcess(active_t childFunc,active_t parentFunc) {
    pid_t pid = fork();

    if(pid == 0){
      int child_PID = getpid();

      printf("child id: %d\n",child_PID);

      saveChildID(child_PID);

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
