/*GRB*
  Gerbera - https://gerbera.io/

  task_worker.h - this file is part of Gerbera.

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

/// \file task_worker.h

#ifndef GERBERA_TASK_WORKER_H
#define GERBERA_TASK_WORKER_H


#include "task_threadpool.h"

class Worker {
public:
    explicit Worker(TaskThreadPool &s);
    void operator()();
private:
    TaskThreadPool &pool;
};



#endif //GERBERA_TASK_WORKER_H
