//
// Created by zyx on 2020/10/14.
//

#ifndef PRCESSANDTHREAD_SRC_FORKPROCESS_H_
#define PRCESSANDTHREAD_SRC_FORKPROCESS_H_

#include <fcntl.h>
typedef void(*active_t)();

typedef struct {
  pid_t pid;
  int status;
  int channel[2];

  active_t proc;
  void *data;
  char *name;
} proc_t;

pid_t fork_new_proc(active_t proc);

extern int last_proc_list;
extern proc_t proc_list[1024];

#endif //PRCESSANDTHREAD_SRC_FORKPROCESS_H_
