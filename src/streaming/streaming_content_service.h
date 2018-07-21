/*GRB*
  Gerbera - https://gerbera.io/

  streaming_content_service.h - this file is part of Gerbera.

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

/// \file streaming_content_service.h

#ifndef GERBERA_STREAMING_CONTENT_SERVICE_H
#define GERBERA_STREAMING_CONTENT_SERVICE_H


#include <config/IConfigManager.h>
#include <task/threadpool.h>
#include <task/task_threadpool.h>

class StreamingContentService {
public:
    StreamingContentService(std::shared_ptr<StreamingOptions>, std::shared_ptr<ThreadPool> threadPool);
    ~StreamingContentService();
    void processConfiguredPlaylists();
protected:
    std::shared_ptr<StreamingOptions> streamingOptions;
    std::shared_ptr<ThreadPool> threadPool;
private:
    void makeTasks(std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>>& playlists);
};


#endif //GERBERA_STREAMING_CONTENT_SERVICE_H
