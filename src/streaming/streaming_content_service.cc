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
    GerberaContentManager *contentManager,
    GerberaStorage *storage) :
    streamingOptions(std::move(streamingOptions)),
    threadPool(std::move(threadPool)),
    downloader(std::move(downloader)),
    contentManager(contentManager),
    storage(storage) {
    // Threads = (Number of Cores - 1) or 1
    size_t numThreads = std::max(std::thread::hardware_concurrency(), 2u) - 1u;
    this->threadPool->start(numThreads);
}

StreamingContentService::~StreamingContentService() {
  if(playlistTimer != nullptr) {
    // TODO: Make GerberaTimer and replace this singleton with instance level reference...
    Timer::getInstance()->removeTimerSubscriber(this, playlistTimer);
  }
}

void StreamingContentService::startupPlaylists() {
  if(this->streamingOptions->playlists()->isUpdateAtStart()) {
    this->processConfiguredPlaylists();
  }
  unsigned int refresh = this->streamingOptions->playlists()->getRefresh();
  if(refresh > 0) {
    playlistTimer = Ref<Timer::Parameter>(new Timer::Parameter(Timer::Parameter::IDOnlineContent, OS_Playlists));
    // TODO: Make GerberaTimer and replace this singleton with instance level reference...
    Timer::getInstance()->addTimerSubscriber(this, refresh, playlistTimer, false);
  }
}

bool StreamingContentService::shouldProcessPlaylist(std::string playlistName, int purgeInterval) {
  bool result;
  std::string playlistContainerChain = this->streamingOptions->playlists()->getRootVirtualPath() + DIR_SEPARATOR + playlistName;
  auto object = this->storage->findVirtualObjectByPath(playlistContainerChain);

  if(object == nullptr || purgeInterval <= 0) {
    log_debug("No playlist object found for container path: %s\n", playlistContainerChain.c_str());
    result = true;
  } else {
    zmm::String lastUpdateTime = object->getAuxData(_(ONLINE_SERVICE_LAST_UPDATE));
    if (!string_ok(lastUpdateTime)) {
      result = true;
    } else {
      result = this->isExpired(lastUpdateTime.toLong(), purgeInterval);
    }
    std::ostringstream msg;
    msg << "Playlist with container path: " << playlistContainerChain.c_str();
    msg << " --> Last Update: " << lastUpdateTime.c_str();
    msg << " is " << (result ? "expired" : "not expired") << "\n";
    log_debug(msg.str().c_str());
  }
  return result;
}

void StreamingContentService::processConfiguredPlaylists() {
  auto playlists = this->streamingOptions->playlists();
  auto remotePlaylists = playlists->getPlaylists();
  makeTasks(remotePlaylists);
}

void StreamingContentService::timerNotify(Ref<Timer::Parameter> parameter) {
  if (parameter == nullptr) {
    return;
  } else {
    if(parameter->whoami() == Timer::Parameter::IDOnlineContent && parameter->getID() == OS_Playlists) {
        this->processConfiguredPlaylists();
    }
  }
}

void StreamingContentService::makeTasks(std::shared_ptr<std::vector<std::unique_ptr<StreamingOptions::ConfiguredPlaylist>>>& configuredPlaylists) {
  for (const auto &playlist : *configuredPlaylists) {
    std::shared_ptr<PlaylistTask> task = std::make_shared<PlaylistTask>(playlist->getUrl(),
            playlist->getName(), playlist->purgeAfter(), this);
    this->threadPool->enqueue(task);
  }
  log_debug("Queue %d tasks for processing configured playlists\n", configuredPlaylists->size());
}

std::shared_ptr<InMemoryPlaylist> StreamingContentService::downloadPlaylist(std::string name, std::string url) {
  auto remoteUrl = std::make_shared<gerbera::URL>(url);
  std::string content = this->downloader->download(remoteUrl);
  log_debug("Downloaded playlist from: %s --> total bytes = %d\n", url.c_str(), content.size());
  return std::make_shared<InMemoryPlaylist>(name, content);
}

std::shared_ptr<PlaylistParseResult> StreamingContentService::parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) {
  auto parentCds = generatePlaylistContainer(playlist->getName());
  auto parseResult = std::make_shared<PlaylistParseResult>(parentCds);

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
      log_error("Unable to identify playlist type from line: \n%s\n", firstLine.c_str());
      playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
    }
  }

  log_debug("Parsed %d playlist items with playlist type: %d\n", playlistItems->size(), type);
  for (const auto &item : *playlistItems) {
    parseResult->addItem(item);
  }

  return parseResult;
}

unsigned long StreamingContentService::persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult, int purgeAfter) {
  unsigned long objectsAdded = 0;

  // Create the wrapping container
  std::string rootVirtualPath = this->streamingOptions->playlists()->getRootVirtualPath();
  int containerId = createRootContainer(rootVirtualPath);

  // Create the playlist container
  auto playlistContainer = parseResult->getParentContainer();
  std::string playlistName = playlistContainer->getTitle().c_str();
  std::string upnpClass = playlistContainer->getClass().c_str();
  auto newContainer = createPlaylistContainer(containerId, rootVirtualPath, playlistName, upnpClass, purgeAfter);

  // Create the child streams
  auto childItems = parseResult->getChildObjects();

  for (auto &cdsObj : *childItems) {
    cdsObj->setParentID(newContainer->getID());
    contentManager->addObject(RefCast(cdsObj, CdsObject));
    objectsAdded++;
  }

  return objectsAdded;
}

int StreamingContentService::createRootContainer(std::string containerChain) {
  std::unique_lock<std::mutex> lock(mutex);

  int containerId;
  log_debug("Searching for root container for configured playlists: %s\n", containerChain.c_str());
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

zmm::Ref<CdsContainer> StreamingContentService::createPlaylistContainer(int parentId, std::string containerName,
        std::string playlistName, std::string upnpClass, int purgeAfter) {

  std::string containerChain = containerName + DIR_SEPARATOR + playlistName;
  auto existingPlaylist = storage->findVirtualObjectByPath(containerChain);
  if(existingPlaylist == nullptr) {
    existingPlaylist = this->newContainer(parentId, playlistName, upnpClass);
  } else {
    bool toDelete;
    zmm::String lastUpdateTime = existingPlaylist->getAuxData(_(ONLINE_SERVICE_LAST_UPDATE));
    if (!string_ok(lastUpdateTime)) {
      toDelete = true;
    } else {
      toDelete = this->isExpired(lastUpdateTime.toLong(), purgeAfter);
    }

    if(toDelete) {
      log_debug("Delete existing playlist %s with container ID: %d --> Last Updated: %s\n", playlistName.c_str(),
              existingPlaylist->getID(), lastUpdateTime.c_str());
      contentManager->removeObject(existingPlaylist->getID(), false, true);
      existingPlaylist = this->newContainer(parentId, playlistName, upnpClass);
    }
  }
  return RefCast(existingPlaylist, CdsContainer);
}

zmm::Ref<CdsObject> StreamingContentService::newContainer(int parentId, std::string name, std::string upnpClass) {
  struct timespec ts;
  getTimespecNow(&ts);
  int containerId = contentManager->addContainer(parentId, name, upnpClass);
  auto newObject = storage->loadObject(containerId);
  newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(ts.tv_sec));
  int containerChanged = INVALID_OBJECT_ID;
  storage->updateObject(newObject, &containerChanged);
  return newObject;
}

std::shared_ptr<CdsContainer> StreamingContentService::generatePlaylistContainer(std::string playlistName) {
  int parentID = -1; // TODO lookup parentId?
  auto parent = std::make_shared<CdsContainer>();
  parent->setClass("object.container");
  parent->setParentID(parentID);
  parent->setTitle(playlistName);
  parent->setVirtual(true);
  // TODO: Lookup existing containerId & cleanup
  return parent;
}

PlaylistType StreamingContentService::determinePlaylistType(std::string firstLine) {
  PlaylistType type;
  std::regex plsRegex(REGEX_PLAYLIST);
  std::regex m3uRegex(REGEX_M3U);
  std::regex xspfRegex(REGEX_XSPF_XML);
  if(std::regex_match(firstLine.c_str(), plsRegex)) {
    type = PLS;
  } else if (std::regex_match(firstLine.c_str(), m3uRegex)) {
    type = M3U;
  } else if (std::regex_match(firstLine.c_str(), xspfRegex)) {
    type = XSPF;
  } else {
    type = UNKNOWN;
  }
  log_debug("Playlist type: %d\n", type);
  return type;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> StreamingContentService::parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist) {
  log_debug("Begin parsing playlist: PLS type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

  std::regex fileRegex(REGEX_PLS_FILE);
  std::regex titleRegex(REGEX_PLS_TITLE);

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, fileRegex)) {
      if(pieces.size() >= 2) {
        std::string location = pieces.str(2);

        // Get the Title
        std::string title;
        line = playlistLines.at(++i);
        if (std::regex_match(line.c_str(), pieces, titleRegex)) {
          title = pieces.str(2);
        } else {
          title = "";
        }

        try {
          auto newObject = createExternalUrl(title, location);
          playlistItems->push_back(newObject);
        } catch (const Exception &ex) {
          log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }
  return playlistItems;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> StreamingContentService::parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist) {
  log_debug("Begin parsing playlist: M3U type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

  std::regex titleRegex(REGEX_M3U_TITLE);

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, titleRegex)) {
      if(pieces.size() >= 4) {
        std::string title = pieces.str(3);
        std::string location = playlistLines.at(++i);
        try {
          auto newObject = createExternalUrl(title, location);
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
  log_debug("Begin parsing playlist: XSPF type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistContent = playlist->getContent();

  Ref<Parser> parser(new Parser());
  zmm::Ref<mxml::Document> rootDoc =  parser->parseString(playlistContent);
  zmm::Ref<mxml::Element> root = rootDoc->getRoot();
  Ref<XPath> rootXPath(new XPath(root));
  Ref<Element> trackList = rootXPath->getElement(XSPF_TRACKLIST);

  Ref<Element> track;
  for (int e = 0; e < trackList->elementChildCount(); e++) {
    track = trackList->getElementChild(e);
    Ref<Element> location = track->getChildByName(XSPF_LOCATION);
    Ref<Element> title = track->getChildByName(XSPF_TITLE);
    std::string titleStr = std::string(title->getText().c_str());
    std::string locationStr = std::string(location->getText().c_str());

    try {
      auto newObject = createExternalUrl(titleStr, locationStr);
      playlistItems->push_back(newObject);
    } catch (const Exception &ex) {
      log_warning("Failed to validate newly created Configured Playlist item: %s\n", ex.getMessage().c_str());
      continue;
    }

  }
  return playlistItems;
}

zmm::Ref<CdsItemExternalURL> StreamingContentService::createExternalUrl(std::string title, std::string location) {
  struct timespec ts;
  auto newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  newObject->setTitle(title);
  newObject->setClass(UPNP_DEFAULT_CLASS_MUSIC_TRACK);

  // Add HTTP Protocol
  zmm::Ref<CdsResource> resource(new CdsResource(CH_DEFAULT));
  String protocolInfo = renderProtocolInfo(MIMETYPE_DEFAULT, PROTOCOL);
  resource->addAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO), protocolInfo);
  newObject->addResource(resource);

  getTimespecNow(&ts);
  newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(ts.tv_sec));
  newObject->setURL(location);
  newObject->setVirtual(true);
  newObject->validate();
  return newObject;
}

bool StreamingContentService::isExpired(long timeSpec, long purgeInterval) {
  bool result;
  struct timespec current, last;
  getTimespecNow(&current);
  last.tv_nsec = 0;
  last.tv_sec = timeSpec;

  long timeDiff = current.tv_sec - last.tv_sec;

  if (timeDiff > purgeInterval) {
    result = true;
  } else {
    result = false;
  }
  log_debug("Item expired = %d with time difference: %d\n", result, timeDiff);
  return result;
}