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
#include <content/abstract_content_manager.h>
#include <storage/abstract_storage.h>
#include "streaming_content.h"
#include "downloader.h"

enum PlaylistType {
    PLS, M3U, XSPF, UNKNOWN
};

class StreamingContentService : public StreamingContent {
public:
    StreamingContentService(std::shared_ptr<StreamingOptions>,
        std::shared_ptr<ThreadPool> threadPool,
        std::shared_ptr<Downloader> curlDownloader,
        AbstractContentManager* contentManager,
        AbstractStorage* storage);
    ~StreamingContentService() = default;
    void processConfiguredPlaylists() override;
    std::shared_ptr<InMemoryPlaylist> downloadPlaylist(std::string name, std::string url) override;
    std::shared_ptr<PlaylistParseResult> parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) override;
    unsigned long persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult) override;
protected:
    std::shared_ptr<StreamingOptions> streamingOptions;
    std::shared_ptr<ThreadPool> threadPool;
    std::shared_ptr<Downloader> downloader;
    AbstractContentManager* contentManager;
    AbstractStorage* storage;
    std::mutex mutex;
private:
    void makeTasks(std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>>& playlists);
    std::shared_ptr<CdsContainer> createPlaylistContainer(std::string playlistName);
    int createRootContainer(std::string containerChain);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseXSPF(std::shared_ptr<InMemoryPlaylist>& playlist);
    PlaylistType determinePlaylistType(std::string firstLine);
};


#endif //GERBERA_STREAMING_CONTENT_SERVICE_H
