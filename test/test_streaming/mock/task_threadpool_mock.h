#ifndef GERBERA_TASK_THREADPOOL_MOCK_H
#define GERBERA_TASK_THREADPOOL_MOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <task/task_threadpool.h>

using namespace ::testing;

class TaskThreadPoolMock : public ThreadPool {
public:
    MOCK_METHOD0(getPoolSize, long long());
    MOCK_METHOD0(getQueueSize, long long());
    MOCK_METHOD0(tasksReceived, long long());
    MOCK_METHOD0(tasksCompleted, long long());
    MOCK_METHOD1(enqueue, void(std::shared_ptr<Task> t));
    MOCK_METHOD1(start, void(int numberOfThreads));
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(isShutdown, bool());
};
#endif //GERBERA_TASK_THREADPOOL_MOCK_H
