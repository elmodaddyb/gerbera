/*GRB*

Gerbera - https://gerbera.io/

    task_threadpool.h - this file is part of Gerbera.

    Copyright (C) 2016-2018 Gerbera Contributors

    Gerbera is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    Gerbera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.

    $Id$
*/

/// \file task_threadpool.h

#ifndef GERBERA_TASKTHREADPOOL_H
#define GERBERA_TASKTHREADPOOL_H

#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <queue>
#include "task.h"

class TaskThreadPool {
public:
    TaskThreadPool();
    virtual ~TaskThreadPool();
    long long getPoolSize();
    long long getQueueSize();
    long long tasksReceived();
    long long tasksCompleted();
    void enqueue(std::unique_ptr<Task> t);
protected:
    std::vector<std::thread> threads;
    std::queue<std::unique_ptr<Task>> tasks;
private:
    friend class Worker;
    std::mutex mutex;
    std::condition_variable cond;
    bool shutdown;
    struct
    {
        unsigned long int received = 0;
        unsigned long int completed = 0;
    } stats;

};

#endif //GERBERA_TASKTHREADPOOL_H
