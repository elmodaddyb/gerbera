/*GRB*

Gerbera - https://gerbera.io/

    task_threadpool.cc - this file is part of Gerbera.

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

/// \file task_threadpool.cc

#include <logger.h>
#include "task_threadpool.h"
#include "task_worker.h"

TaskThreadPool::~TaskThreadPool() {
  if(!shutdown) {
    this->stop();
  }
}

void TaskThreadPool::start(size_t numberOfThreads) {
  std::unique_lock<std::mutex> lock(mutex);
  if(shutdown) {
    log_debug("Starting thread pool with %d threads\n", numberOfThreads);
    shutdown = false;
    for (size_t i = 0; i < numberOfThreads; i++) {
      threads.emplace_back(std::thread(Worker(*this)));
    }
  }
}

void TaskThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(mutex);
    shutdown = true;
  }
  cond.notify_all();
  for(std::thread &thread: threads) {
    thread.join();
  }
  threads.clear();
}

void TaskThreadPool::enqueue(std::shared_ptr<Task> t)
{
  {
    std::unique_lock<std::mutex> lock(mutex);

    if(shutdown) {
      throw std::runtime_error("Attemped to enqueue when TaskThreadPool is shutdown");
    }
    stats.received++;
    tasks.emplace(std::move(t));
  }
  cond.notify_one();
}

bool TaskThreadPool::isShutdown() {
  std::lock_guard<std::mutex> lock(mutex);
  return this->shutdown;
}

size_t TaskThreadPool::tasksReceived() {
  std::lock_guard<std::mutex> lock(mutex);
  return this->stats.received;
}

size_t TaskThreadPool::tasksCompleted() {
  std::lock_guard<std::mutex> lock(mutex);
  return this->stats.completed;
}

size_t TaskThreadPool::getPoolSize() {
  std::lock_guard<std::mutex> lock(mutex);
  return this->threads.size();
}

size_t TaskThreadPool::getQueueSize() {
  std::lock_guard<std::mutex> lock(mutex);
  return this->tasks.size();
}


