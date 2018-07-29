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
#include <regex>
#include <metadata_handler.h>
#include <tools.h>
#include <online_service.h>
#include "streaming_content_service.h"
#include "playlist_task.h"

using namespace zmm;

StreamingContentService::StreamingContentService(
    std::shared_ptr<StreamingOptions> streamingOptions,
    std::shared_ptr<ThreadPool> threadPool,
    std::shared_ptr<CurlDownloader> curlDownloader,
    AbstractContentManager *contentManager,
    AbstractStorage *storage) :
    streamingOptions(std::move(streamingOptions)),
    threadPool(std::move(threadPool)),
    curlDownloader(std::move(curlDownloader)),
    contentManager(contentManager),
    storage(storage) {
  int numCores = std::thread::hardware_concurrency();
  this->threadPool->start(numCores - 2);
}

void StreamingContentService::processConfiguredPlaylists() {
  std::shared_ptr<StreamingPlaylists> playlists = this->streamingOptions->getPlaylists();
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> remotePlaylists = playlists->getPlaylists();
  makeTasks(remotePlaylists);
}

void StreamingContentService::makeTasks(std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> &remotePlaylists) {
  for (const auto &playlist : (*remotePlaylists)) {
    std::shared_ptr<PlaylistTask> task = std::make_shared<PlaylistTask>(playlist->getUrl(), this);
    this->threadPool->enqueue(task);
  }
}

void StreamingContentService::printUrl(std::string url) {
  printf("\n%s\n", url.c_str());
}

std::shared_ptr<InMemoryPlaylist> StreamingContentService::downloadPlaylist(std::string url) {
  std::string content = this->curlDownloader->download(url);
  return std::make_shared<InMemoryPlaylist>(content);
}

std::shared_ptr<PlaylistParseResult> StreamingContentService::parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) {
  struct timespec ts;
  auto parentCds = createParentContainer("Radio Playlist");
  auto parseResult = std::make_shared<PlaylistParseResult>(parentCds);

  std::vector<std::string> playlistLines = playlist->getContentVector();

  std::regex fileRegex("^File(\\d)+?=(.*)$");
  std::regex titleRegex("^Title(\\d)+?=(.*)$");

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, fileRegex)) {
      Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
      if(pieces.size() >= 2) {


        newObject->setURL(pieces.str(2));
        newObject->setClass("object.item.audioItem");

        // Add HTTP Protocol
        Ref<CdsResource> resource(new CdsResource(CH_DEFAULT));
        String protocolInfo = renderProtocolInfo(MIMETYPE_DEFAULT, PROTOCOL);
        resource->addAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO), protocolInfo);
        newObject->addResource(resource);

        // Add last update timestamp
        getTimespecNow(&ts);
        newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(ts.tv_sec));

        // Get the Title
        line = playlistLines.at(++i);
        if (std::regex_match(line.c_str(), pieces, titleRegex)) {
          newObject->setTitle(pieces.str(2));
        }

        // Add the item
        try {
          newObject->validate();
          newObject->setVirtual(true);
          parseResult->addItem(newObject);
        }
        catch (const Exception &ex) {
          log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }

  return parseResult;
}

unsigned long StreamingContentService::persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult) {
  unsigned long objectsAdded = 0;

  std::shared_ptr<CdsContainer> parentContainer = parseResult->getParentContainer();
  int containerId = contentManager->addContainer(parentContainer->getParentID(), parentContainer->getTitle(), parentContainer->getClass());
  Ref<CdsObject> parentObj = storage->loadObject(containerId);
  objectsAdded++;

  auto childItems = parseResult->getChildObjects();

  for (auto &cdsObj : (*childItems)) {
    cdsObj->setParentID(parentObj->getParentID());
    contentManager->addObject(RefCast(cdsObj, CdsObject));
    objectsAdded++;
  }

  return objectsAdded;
}

std::shared_ptr<CdsContainer> StreamingContentService::createParentContainer(std::string containerName) {
  int parentID = 0; // TODO lookup parentId?
  std::shared_ptr<CdsContainer> parent = std::make_shared<CdsContainer>();
  parent->setClass("object.container");
  parent->setParentID(parentID);
  parent->setTitle(containerName);
  // TODO: Lookup existing containerId & cleanup
  return parent;
}

