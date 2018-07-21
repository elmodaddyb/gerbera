//
// Created by Eamonn Buss on 7/14/18.
//

#ifndef GERBERA_THREADPOOL_H
#define GERBERA_THREADPOOL_H

#include <memory>
#include "task.h"

class ThreadPool {

public:
    virtual long long getPoolSize() = 0;

    virtual long long getQueueSize() = 0;

    virtual long long tasksReceived() = 0;

    virtual long long tasksCompleted() = 0;

    virtual void enqueue(std::shared_ptr<Task> t) = 0;

    virtual bool isShutdown() = 0;

    virtual void start(int numberOfThreads) = 0;

    virtual void stop() = 0;

    virtual ~ThreadPool() {}
};
#endif //GERBERA_THREADPOOL_H
