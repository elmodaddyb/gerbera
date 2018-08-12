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
#include <algorithm>
#include <metadata_handler.h>
#include <tools.h>
#include <online_service.h>
#include <gerbera/url.h>
#include <xpath.h>
#include "streaming_content_service.h"
#include "playlist_task.h"

using namespace zmm;

StreamingContentService::StreamingContentService(
    std::shared_ptr<StreamingOptions> streamingOptions,
    std::shared_ptr<ThreadPool> threadPool,
    std::shared_ptr<Downloader> downloader,
    AbstractContentManager *contentManager,
    AbstractStorage *storage) :
    streamingOptions(std::move(streamingOptions)),
    threadPool(std::move(threadPool)),
    downloader(std::move(downloader)),
    contentManager(contentManager),
    storage(storage) {
  int numCores = std::thread::hardware_concurrency();
  this->threadPool->start(numCores - 2); // TODO make this number right...
}

void StreamingContentService::processConfiguredPlaylists() {
  std::shared_ptr<StreamingPlaylists> playlists = this->streamingOptions->getPlaylists();
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> remotePlaylists = playlists->getPlaylists();
  makeTasks(remotePlaylists);
}

void StreamingContentService::makeTasks(std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> &remotePlaylists) {
  for (const auto &playlist : *remotePlaylists) {
    std::shared_ptr<PlaylistTask> task = std::make_shared<PlaylistTask>(playlist->getUrl(), playlist->getName(), this);
    this->threadPool->enqueue(task);
  }
}

std::shared_ptr<InMemoryPlaylist> StreamingContentService::downloadPlaylist(std::string name, std::string url) {
  auto remoteUrl = std::make_shared<gerbera::URL>(url);
  std::string content = this->downloader->download(remoteUrl);
  return std::make_shared<InMemoryPlaylist>(name, content);
}

std::shared_ptr<PlaylistParseResult> StreamingContentService::parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) {
  auto parentCds = createPlaylistContainer(playlist->getName());
  auto parseResult = std::make_shared<PlaylistParseResult>(parentCds);

  // Determine the type of playlist
  std::string firstLine = playlist->getContentVector().at(0);
  PlaylistType type = determinePlaylistType(firstLine);
  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> playlistItems;
  switch(type) {
    case PLS: {
      playlistItems = parsePLS(playlist);
      break;
    }
    case M3U: {
      playlistItems = parseM3U(playlist);
      break;
    }
    case XSPF: {
      playlistItems = parseXSPF(playlist);
      break;
    }
    default: {
      playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
    }
  }

  // Add items to the parseResult
  for (const auto &item : *playlistItems) {
    parseResult->addItem(item);
  }

  return parseResult;
}

unsigned long StreamingContentService::persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult) {
  unsigned long objectsAdded = 0;

  // Create the wrapping container
  std::string rootVirtualPath = this->streamingOptions->getPlaylists()->getRootVirtualPath();
  int containerId = createRootContainer(rootVirtualPath);

  // Create the playlist container
  std::shared_ptr<CdsContainer> playlistContainer = parseResult->getParentContainer();
  int playlistContainerId = contentManager->addContainer(containerId, playlistContainer->getTitle(), playlistContainer->getClass());
  playlistContainer->setParentID(containerId);
  playlistContainer->setID(playlistContainerId);

  // Create the child streams
  auto childItems = parseResult->getChildObjects();

  for (auto &cdsObj : *childItems) {
    cdsObj->setParentID(playlistContainer->getID());
    contentManager->addObject(RefCast(cdsObj, CdsObject));
    objectsAdded++;
  }

  return objectsAdded;
}

int StreamingContentService::createRootContainer(std::string containerChain) {
  std::unique_lock<std::mutex> lock(mutex);

  int containerId;
  log_debug("Searching for root container for configured playlists: %s", containerChain.c_str());
  zmm::Ref<CdsObject> object = this->storage->findVirtualObjectByPath(containerChain);

  if(object == nullptr) {
    containerId = this->contentManager->addContainerChain(containerChain, nullptr, INVALID_OBJECT_ID, nullptr);
    log_debug("Added new root container: %d\n", containerId);
  } else {
    containerId = object->getID();
    log_debug("Found existing root container: %d\n", containerId);
  }

  lock.unlock();
  return containerId;
}

std::shared_ptr<CdsContainer> StreamingContentService::createPlaylistContainer(std::string playlistName) {
  int parentID = 0; // TODO lookup parentId?
  std::shared_ptr<CdsContainer> parent = std::make_shared<CdsContainer>();
  parent->setClass("object.container");
  parent->setParentID(parentID);
  parent->setTitle(playlistName);
  parent->setVirtual(true);
  // TODO: Lookup existing containerId & cleanup
  return parent;
}

PlaylistType StreamingContentService::determinePlaylistType(std::string firstLine) {
  PlaylistType type;
  std::regex plsRegex("^\\[playlist\\]$");
  std::regex m3uRegex("^#EXTM3U$");
  std::regex xspfRegex(".*xmlns=\"http://xspf.org/ns/0/\".*");
  std::cmatch pieces;
  if(std::regex_match(firstLine.c_str(), pieces, plsRegex)) {
    type = PLS;
  } else if (std::regex_match(firstLine.c_str(), pieces, m3uRegex)) {
    type = M3U;
  } else if (std::regex_match(firstLine.c_str(), pieces, xspfRegex)) {
    type = XSPF;
  } else {
    type = UNKNOWN;
  }
  return type;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> StreamingContentService::parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist) {
  struct timespec ts;
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

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

        try {
          newObject->validate();
          newObject->setVirtual(true);
          playlistItems->push_back(newObject);
        }
        catch (const Exception &ex) {
          log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }
  return playlistItems;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> StreamingContentService::parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist) {
  struct timespec ts;
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

  std::regex titleRegex("^(#EXTINF:)([-]?[\\d]+),(.*)$");

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, titleRegex)) {
      Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
      if(pieces.size() >= 4) {
        newObject->setTitle(pieces.str(3));
        newObject->setClass("object.item.audioItem");

        // Add HTTP Protocol
        Ref<CdsResource> resource(new CdsResource(CH_DEFAULT));
        String protocolInfo = renderProtocolInfo(MIMETYPE_DEFAULT, PROTOCOL);
        resource->addAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO), protocolInfo);
        newObject->addResource(resource);

        // Add last update timestamp
        getTimespecNow(&ts);
        newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(ts.tv_sec));

        // Get the URL
        line = playlistLines.at(++i);
        newObject->setURL(line);

        try {
          newObject->validate();
          newObject->setVirtual(true);
          playlistItems->push_back(newObject);
        }
        catch (const Exception &ex) {
          log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }
  return playlistItems;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> StreamingContentService::parseXSPF(std::shared_ptr<InMemoryPlaylist>& playlist) {
  struct timespec ts;
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistContent = playlist->getContent();

  Ref<Parser> parser(new Parser());
  zmm::Ref<mxml::Document> rootDoc =  parser->parseString(playlistContent);
  zmm::Ref<mxml::Element> root = rootDoc->getRoot();
  Ref<XPath> rootXPath(new XPath(root));
  Ref<Element> trackList = rootXPath->getElement("/trackList");

  Ref<Element> track;
  for (int e = 0; e < trackList->elementChildCount(); e++) {
    track = trackList->getElementChild(e);
    Ref<Element> location = track->getChildByName("location");
    Ref<Element> title = track->getChildByName("title");

    Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
    newObject->setTitle(title->getText());
    newObject->setClass("object.item.audioItem");

    // Add HTTP Protocol
    Ref<CdsResource> resource(new CdsResource(CH_DEFAULT));
    String protocolInfo = renderProtocolInfo(MIMETYPE_DEFAULT, PROTOCOL);
    resource->addAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO), protocolInfo);
    newObject->addResource(resource);

    // Add last update timestamp
    getTimespecNow(&ts);
    newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(ts.tv_sec));

    // Get the URL
    newObject->setURL(location->getText());

    try {
      newObject->validate();
      newObject->setVirtual(true);
      playlistItems->push_back(newObject);
    }
    catch (const Exception &ex) {
      log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
      continue;
    }

  }
  return playlistItems;
}