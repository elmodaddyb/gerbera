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
#include <memory>
#include <algorithm>
#include <gerbera/url.h>
#include <online_service.h>

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
    this->playlistParser = std::make_unique<PlaylistParser>();
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

  if(object == nullptr) {
    log_debug("No playlist object found for container path: %s\n", playlistContainerChain.c_str());
    result = true;
  } else if (purgeInterval >= 0) {
    zmm::String lastUpdateTime = object->getAuxData(_(ONLINE_SERVICE_LAST_UPDATE));
    if (!string_ok(lastUpdateTime)) {
      result = false;
    } else {
      result = this->isExpired(lastUpdateTime.toLong(), purgeInterval);
    }
    std::ostringstream msg;
    msg << "Playlist with container path: " << playlistContainerChain.c_str();
    msg << " --> Last Update: " << lastUpdateTime.c_str();
    msg << " is " << (result ? "expired" : "not expired") << "\n";
    log_debug(msg.str().c_str());
  } else {
    result = false;
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

std::shared_ptr<PlaylistParseResult> StreamingContentService::parsePlaylist(std::shared_ptr<InMemoryPlaylist>& playlist) {
  auto parentCds = generatePlaylistContainer(playlist->getName());
  auto parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> playlistItems = playlistParser->parsePlaylist(playlist);
  for (const auto &item : *playlistItems) {
    parseResult->addItem(item);
  }
  return parseResult;
}

unsigned long StreamingContentService::persistPlaylist(std::shared_ptr<PlaylistParseResult>& parseResult, int purgeAfter) {
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