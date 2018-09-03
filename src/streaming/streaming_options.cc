/*GRB*
  Gerbera - https://gerbera.io/

  streaming_options.cc - this file is part of Gerbera.

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

/// \file streaming_options.cc

#include "streaming_options.h"
#include <vector>

using namespace zmm;

StreamingOptions::StreamingOptions(Ref<Element> streamingConfig) {
  Ref<Element> cfgPlaylists = streamingConfig->getChildByName(_("playlists"));
  std::string rootVirtualPath = cfgPlaylists->getAttribute("root-virtual-path").c_str();
  bool updateStart = cfgPlaylists->getAttribute("update-at-start") == "yes" != 0;
  String refreshStr = cfgPlaylists->getAttribute("refresh");
  unsigned int refresh;
  if(refreshStr != nullptr) {
    refresh = ((unsigned)std::stoi(refreshStr.c_str()));
  } else {
    refresh = 0;
  }
  _playlists = std::make_shared<Playlists>(rootVirtualPath, updateStart, refresh);
  std::unique_ptr<ConfiguredPlaylist> playlist;
  Ref<Element> item;
  for (int e = 0; e < cfgPlaylists->elementChildCount(); e++) {
    item = cfgPlaylists->getElementChild(e);
    std::string url = item->getAttribute(_("url")).c_str();
    std::string name = item->getAttribute(_("name")).c_str();

    int purgeAfter;
    if(item->getAttribute(_("purge-after")) == nullptr) {
      purgeAfter = -1;
    } else {
      std::string purgeAfterStr = item->getAttribute(_("purge-after")).c_str();
      try {
        purgeAfter = std::stoi(purgeAfterStr);
      }
      catch (const std::invalid_argument& ia) {
        log_error("`playlist/attribute::purge-after=%s must be integer -- using default value -1\n", purgeAfterStr.c_str());
        purgeAfter = -1;
      }
    }

    playlist = std::make_unique<ConfiguredPlaylist>(url, name, purgeAfter);
    _playlists->addPlaylist(std::move(playlist));
  }

  Ref<Element> cfgShoutcast = streamingConfig->getChildByName(_("shoutcast"));
  std::string baseUrl = cfgShoutcast->getAttribute(_("base-url")).c_str();
  std::string devId = cfgShoutcast->getAttribute(_("dev-id")).c_str();
  std::string enabled = cfgShoutcast->getAttribute(_("enabled")).c_str();
  _shoutcast = std::make_shared<Shoutcast>(baseUrl, devId, enabled == "yes");
}

StreamingOptions::~StreamingOptions() {
  _playlists = nullptr;
  _shoutcast = nullptr;
}

StreamingOptions::ConfiguredPlaylist::ConfiguredPlaylist(std::string url, std::string name, int purgeAfter = -1) :
_url(std::move(url)), _name(std::move(name)), _purgeAfter(purgeAfter){}

std::string StreamingOptions::ConfiguredPlaylist::getUrl() {
  return this->_url;
}

std::string StreamingOptions::ConfiguredPlaylist::getName() {
  return this->_name;
}

std::shared_ptr<StreamingOptions::Playlists> StreamingOptions::playlists() {
  return _playlists;
}

std::shared_ptr<StreamingOptions::Shoutcast> StreamingOptions::shoutcastOptions() {
  return _shoutcast;
}

StreamingOptions::Playlists::Playlists(std::string rootVirtualPath, bool updateAtStart, unsigned int refresh):
  _rootVirtualPath(std::move(rootVirtualPath)), _updateAtStart(updateAtStart), _refresh(refresh) {
  confPlaylists = std::make_shared<std::vector<std::unique_ptr<ConfiguredPlaylist>>>();
}

unsigned long StreamingOptions::Playlists::getSize() {
  return getPlaylists()->size();
}

void StreamingOptions::Playlists::addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist) {
  confPlaylists->push_back(std::move(playlist));
}

std::shared_ptr<std::vector<std::unique_ptr<StreamingOptions::ConfiguredPlaylist>>> StreamingOptions::Playlists::getPlaylists() {
  return confPlaylists;
}

std::string StreamingOptions::Playlists::getRootVirtualPath() {
  return _rootVirtualPath;
}

bool StreamingOptions::Playlists::isUpdateAtStart() {
  return _updateAtStart;
}

unsigned int StreamingOptions::Playlists::getRefresh() {
  return _refresh;
}

StreamingOptions::Shoutcast::Shoutcast(std::string baseUrl, std::string devId, bool enabled) :
        _baseUrl(std::move(baseUrl)), _devId(std::move(devId)), _enabled(enabled) { }

std::string StreamingOptions::Shoutcast::getBaseUrl() {
  return _baseUrl;
}

std::string StreamingOptions::Shoutcast::getDevId() {
  return _devId;
}

bool StreamingOptions::Shoutcast::isEnabled() {
  return _enabled;
}

int StreamingOptions::ConfiguredPlaylist::purgeAfter() {
  return _purgeAfter;
}
