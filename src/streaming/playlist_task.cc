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

PlaylistTask::PlaylistTask(std::string url, StreamingContent* streamingContentService) :
        streamingContentService(streamingContentService), url(std::move(url)){
}

void PlaylistTask::run() {
  auto inMemoryPlaylist = this->streamingContentService->downloadPlaylist(this->url);
  auto parseResult = this->streamingContentService->parsePlaylist(inMemoryPlaylist);
  unsigned long itemsAdded = this->streamingContentService->persistPlaylist(parseResult);

  std::ostringstream completionMsg;
  completionMsg << "\n";
  completionMsg << "Playlist Task : COMPLETE\n";
  completionMsg << "Playlist Name : `" << parseResult->getParentContainer()->getTitle() << "`\n";
  completionMsg << "Playlist Items: " << itemsAdded << "\n";
  log_info(completionMsg.str().c_str());
}

std::string PlaylistTask::getUrl() {
  return this->url;
}
