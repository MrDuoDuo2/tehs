//
// Created by zyx on 2020/10/14.
//

#ifndef PRCESSANDTHREAD_SRC_FORKPROCESS_H_
#define PRCESSANDTHREAD_SRC_FORKPROCESS_H_

typedef void(*active_t)();

void fork_new_proc(active_t childFunc, active_t parentFunc);

void fork_new_proc(active_t childFunc);


#endif //PRCESSANDTHREAD_SRC_FORKPROCESS_H_
