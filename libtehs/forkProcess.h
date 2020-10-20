//
// Created by zyx on 2020/10/14.
//

#ifndef PRCESSANDTHREAD_SRC_FORKPROCESS_H_
#define PRCESSANDTHREAD_SRC_FORKPROCESS_H_

typedef void(*active_t)();

class forkProcess {

 public:
  forkProcess(active_t childFunc, active_t parentFunc);
};

#endif //PRCESSANDTHREAD_SRC_FORKPROCESS_H_
