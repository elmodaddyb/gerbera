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

using namespace zmm;

StreamingOptions::StreamingOptions(Ref<Element> streamingConfig) {


  Ref<Element> cfgPlaylists = streamingConfig->getChildByName(_("playlists"));
  std::string rootVirtualPath = cfgPlaylists->getAttribute("root-virtual-path").c_str();
  playlists = std::make_shared<StreamingPlaylists>(rootVirtualPath);
  std::unique_ptr<ConfiguredPlaylist> playlist;
  Ref<Element> item;
  for (int e = 0; e < cfgPlaylists->elementChildCount(); e++) {
    item = cfgPlaylists->getElementChild(e);
    playlist = std::make_unique<ConfiguredPlaylist>(item->getAttribute(_("url")).c_str(), item->getAttribute(_("name")).c_str());
    playlists->addPlaylist(std::move(playlist));
  }

  Ref<Element> cfgShoutcast = streamingConfig->getChildByName(_("shoutcast"));
  std::string baseUrl = cfgShoutcast->getAttribute(_("base-url")).c_str();
  std::string devId = cfgShoutcast->getAttribute(_("dev-id")).c_str();
  std::string enabled = cfgShoutcast->getAttribute(_("enabled")).c_str();
  shoutcast = std::make_shared<ShoutcastOptions>(baseUrl, devId, enabled.compare("yes") == 0);
}

StreamingOptions::~StreamingOptions() {
  playlists = nullptr;
  shoutcast = nullptr;
}


std::shared_ptr<StreamingPlaylists> StreamingOptions::getPlaylists() {
  return playlists;
}

std::shared_ptr<ShoutcastOptions> StreamingOptions::getShoutcastOptions() {
  return shoutcast;
}