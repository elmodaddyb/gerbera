//
// Created by Eamonn Buss on 7/8/18.
//

#ifndef GERBERA_TASK_WORKER_H
#define GERBERA_TASK_WORKER_H


#include "task_threadpool.h"

class Worker {
public:
    Worker(TaskThreadPool &s);
    void operator()();
private:
    TaskThreadPool &pool;
};



#endif //GERBERA_TASK_WORKER_H
