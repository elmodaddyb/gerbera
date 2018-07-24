/*GRB*
  Gerbera - https://gerbera.io/

  streaming_content_service.cc - this file is part of Gerbera.

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

/// \file streaming_content_service.cc

#include <thread>
#include "streaming_content_service.h"
#include "playlist_task.h"

using namespace zmm;

StreamingContentService::StreamingContentService(
        std::shared_ptr<StreamingOptions> streamingOptions,
        std::shared_ptr<ThreadPool> threadPool) :
        streamingOptions(std::move(streamingOptions)), threadPool(std::move(threadPool)) {
  int numCores = std::thread::hardware_concurrency();
  this->threadPool->start(numCores - 2);
}

void StreamingContentService::processConfiguredPlaylists() {
  std::shared_ptr<StreamingPlaylists> playlists = this->streamingOptions->getPlaylists();
  std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>> remotePlaylists = playlists->getPlaylists();
  makeTasks(remotePlaylists);
}

void StreamingContentService::makeTasks(std::shared_ptr<std::vector<std::unique_ptr<RemotePlaylist>>> &remotePlaylists) {
  for (const auto &playlist : (*remotePlaylists)) {
    std::shared_ptr<PlaylistTask> task = std::make_shared<PlaylistTask>(playlist->getUrl(), this);
    this->threadPool->enqueue(task);
  }
}

void StreamingContentService::printUrl(std::string url) {
  printf("\n%s\n", url.c_str());
}

