/*GRB*
  Gerbera - https://gerbera.io/

  playlist_task.h - this file is part of Gerbera.

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

/// \file playlist_task.h

#ifndef GERBERA_PLAYLISTTASK_H
#define GERBERA_PLAYLISTTASK_H


#include <string>
#include <task/task.h>
#include "streaming_content_service.h"

class PlaylistTask : public Task {
public:
    PlaylistTask(std::string url, std::string name, StreamingContent* streamingContentService);
    ~PlaylistTask() = default;
    void run() override;

private:
    StreamingContent* streamingContentService;
    std::string url;
    std::string name;
};


#endif //GERBERA_PLAYLISTTASK_H
