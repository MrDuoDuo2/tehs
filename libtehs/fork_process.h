//
// Created by zyx on 2020/10/14.
//

#ifndef PRCESSANDTHREAD_SRC_FORKPROCESS_H_
#define PRCESSANDTHREAD_SRC_FORKPROCESS_H_

using namespace std;

typedef void(*active_t)();

void fork_new_proc(active_t childFunc, active_t parentFunc);

void fork_new_proc(active_t childFunc);
void save_child_id(string home,__pid_t pid,string desc);

#endif //PRCESSANDTHREAD_SRC_FORKPROCESS_H_
