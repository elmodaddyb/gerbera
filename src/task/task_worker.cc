/*GRB*
  Gerbera - https://gerbera.io/

  task_worker.cc - this file is part of Gerbera.

  Copyright (C) 2018 Gerbera Contributors

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

/// \file task_worker.cc

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