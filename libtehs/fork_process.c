//
// Created by zyx on 2020/10/14.
//

#include "fork_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <zconf.h>

int last_proc_list;
proc_t proc_list[1024];

pid_t fork_new_proc(active_t proc) {
  int s;
  pid_t pid;

  for (s = 0; s < last_proc_list; ++s) {
    if (proc_list[s].pid == -1) {
      break;
    }
  }

  if(s==1024){
    printf("proc size 1024");
    return -1;
  }

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, proc_list[s].channel) == -1)
  {
    printf("socket filed");
    return -1;
  }

  pid = fork();

  switch (pid) {
    case 0:
      proc();
      break;
    case -1:
      printf("FORK FAILED\n");
      break;
    default:
      break;
  }

  proc_list[s].pid = pid;
  proc_list[s].proc = proc;
  proc_list[s].name = "";

  if(s == last_proc_list){
    last_proc_list++;
  }

  return pid;
}
