/*GRB*
  Gerbera - https://gerbera.io/

  playlist_task.cc - this file is part of Gerbera.

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

/// \file playlist_task.cc

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "playlist_task.h"

PlaylistTask::PlaylistTask(std::string url, std::string name, int purgeAfter, StreamingContent* streamingContentService) :
        streamingContentService(streamingContentService), url(std::move(url)), name(std::move(name)), purgeAfter(purgeAfter){
}

void PlaylistTask::run() {
  if(this->streamingContentService->shouldProcessPlaylist(this->name, this->purgeAfter)) {
    auto inMemoryPlaylist = this->streamingContentService->downloadPlaylist(this->name, this->url);
    auto parseResult = this->streamingContentService->parsePlaylist(inMemoryPlaylist);
    unsigned long itemsAdded = this->streamingContentService->persistPlaylist(parseResult, this->purgeAfter);

    std::ostringstream completionMsg;
    completionMsg << "Playlist Task : COMPLETE - ";
    completionMsg << "`" << parseResult->getParentContainer()->getTitle() << "` ";
    completionMsg << "added " << itemsAdded << " items\n";
    log_info(completionMsg.str().c_str());
  }
}
