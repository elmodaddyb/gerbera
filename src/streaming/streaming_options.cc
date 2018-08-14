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
  _playlists = std::make_shared<Playlists>(rootVirtualPath);
  std::unique_ptr<ConfiguredPlaylist> playlist;
  Ref<Element> item;
  for (int e = 0; e < cfgPlaylists->elementChildCount(); e++) {
    item = cfgPlaylists->getElementChild(e);
    playlist = std::make_unique<ConfiguredPlaylist>(item->getAttribute(_("url")).c_str(), item->getAttribute(_("name")).c_str());
    _playlists->addPlaylist(std::move(playlist));
  }

  Ref<Element> cfgShoutcast = streamingConfig->getChildByName(_("shoutcast"));
  std::string baseUrl = cfgShoutcast->getAttribute(_("base-url")).c_str();
  std::string devId = cfgShoutcast->getAttribute(_("dev-id")).c_str();
  std::string enabled = cfgShoutcast->getAttribute(_("enabled")).c_str();
  shoutcast = std::make_shared<Shoutcast>(baseUrl, devId, enabled.compare("yes") == 0);
}

StreamingOptions::~StreamingOptions() {
  _playlists = nullptr;
  shoutcast = nullptr;
}

StreamingOptions::ConfiguredPlaylist::ConfiguredPlaylist(std::string url, std::string name) :
url(std::move(url)), name(std::move(name)) {}

std::string StreamingOptions::ConfiguredPlaylist::getUrl() {
  return this->url;
}

std::string StreamingOptions::ConfiguredPlaylist::getName() {
  return this->name;
}

std::shared_ptr<StreamingOptions::Playlists> StreamingOptions::playlists() {
  return _playlists;
}

std::shared_ptr<StreamingOptions::Shoutcast> StreamingOptions::shoutcastOptions() {
  return shoutcast;
}

StreamingOptions::Playlists::Playlists(std::string rootVirtualPath): rootVirtualPath(std::move(rootVirtualPath)) {
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
  return rootVirtualPath;
}

StreamingOptions::Shoutcast::Shoutcast(std::string baseUrl, std::string devId, bool enabled) :
        baseUrl(std::move(baseUrl)), devId(std::move(devId)), enabled(enabled) { }

std::string StreamingOptions::Shoutcast::getBaseUrl() {
  return baseUrl;
}

std::string StreamingOptions::Shoutcast::getDevId() {
  return devId;
}

bool StreamingOptions::Shoutcast::isEnabled() {
  return enabled;
}

