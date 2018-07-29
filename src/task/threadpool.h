/*GRB*
  Gerbera - https://gerbera.io/

  threadpool.h - this file is part of Gerbera.

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

/// \file threadpool.h

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

    virtual void start(size_t numberOfThreads) = 0;

    virtual void stop() = 0;

    virtual ~ThreadPool() = default;
};
#endif //GERBERA_THREADPOOL_H
