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


#include <config/gerbera_conf_manager.h>
#include <task/threadpool.h>
#include <content/gerbera_content_manager.h>
#include <storage/gerbera_storage.h>
#include <timer.h>
#include "streaming_content.h"
#include "downloader.h"

#define REGEX_PLAYLIST  "^\\[playlist\\]$"
#define REGEX_PLS_FILE  "^File(\\d)+?=(.*)$"
#define REGEX_PLS_TITLE "^Title(\\d)+?=(.*)$"
#define REGEX_M3U       "^#EXTM3U$"
#define REGEX_M3U_TITLE "^(#EXTINF:)([-]?[\\d]+),(.*)$"
#define REGEX_XSPF_XML  ".*xmlns=\"http://xspf.org/ns/0/\".*"
#define XSPF_TRACKLIST  "/trackList"
#define XSPF_LOCATION   "location"
#define XSPF_TITLE      "title"

enum PlaylistType {
    PLS, M3U, XSPF, UNKNOWN
};

class StreamingContentService : public StreamingContent, public Timer::Subscriber {
public:
    StreamingContentService(std::shared_ptr<StreamingOptions>,
        std::shared_ptr<ThreadPool> threadPool,
        std::shared_ptr<Downloader> curlDownloader,
        GerberaContentManager* contentManager,
        GerberaStorage* storage);
    ~StreamingContentService() override;
    bool shouldProcessPlaylist(std::string name, int purgeInterval) override;
    void processConfiguredPlaylists() override;
    void startupPlaylists() override;
    std::shared_ptr<InMemoryPlaylist> downloadPlaylist(std::string name, std::string url) override;
    std::shared_ptr<PlaylistParseResult> parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) override;
    unsigned long persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult, int purgeAfter) override;
    void timerNotify(zmm::Ref<Timer::Parameter> parameter) override;
protected:
    std::shared_ptr<StreamingOptions> streamingOptions;
    std::shared_ptr<ThreadPool> threadPool;
    std::shared_ptr<Downloader> downloader;
    GerberaContentManager* contentManager;
    GerberaStorage* storage;
    std::mutex mutex;
    zmm::Ref<Timer::Parameter> playlistTimer;
private:
    void makeTasks(std::shared_ptr<std::vector<std::unique_ptr<StreamingOptions::ConfiguredPlaylist>>>& playlists);
    std::shared_ptr<CdsContainer> generatePlaylistContainer(std::string playlistName);
    int createRootContainer(std::string containerChain);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseXSPF(std::shared_ptr<InMemoryPlaylist>& playlist);
    PlaylistType determinePlaylistType(std::string firstLine);
    zmm::Ref<CdsContainer> createPlaylistContainer(int parentId, std::string containerName, std::string playlistName,
            std::string upnpClass, int purgeAfter);
    zmm::Ref<CdsObject> newContainer(int parentId, std::string playlistName, std::string upnpClass);
    zmm::Ref<CdsItemExternalURL> createExternalUrl(std::string title, std::string location);
    bool isExpired(long playlistTs, long purgeInterval);
};


#endif //GERBERA_STREAMING_CONTENT_SERVICE_H
