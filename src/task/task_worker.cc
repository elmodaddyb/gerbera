//
// Created by Eamonn Buss on 7/8/18.
//

#include "task_worker.h"
#include "task_threadpool.h"

Worker::Worker(TaskThreadPool &s): pool(s) {};

void Worker::operator()() {
  while(true) {
    std::shared_ptr<Task> task;
    {
      std::unique_lock<std::mutex> lock(pool.mutex);
      pool.cond.wait(lock, [this]{ return pool.shutdown || !pool.tasks.empty(); });
      if(pool.shutdown && pool.tasks.empty()) {
        return;
      }
      task = std::move(pool.tasks.front());
      pool.tasks.pop();
    }

    task->run();
    pool.stats.completed++;
  }
}