/*GRB*
  Gerbera - https://gerbera.io/

  streaming_playlists.cc - this file is part of Gerbera.

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

/// \file streaming_playlists.cc

#include "streaming_playlists.h"

StreamingPlaylists::StreamingPlaylists(std::string rootVirtualPath): rootVirtualPath(std::move(rootVirtualPath)) {
  playlists = std::make_shared<std::vector<std::unique_ptr<ConfiguredPlaylist>>>();
}

StreamingPlaylists::~StreamingPlaylists() {}

unsigned long StreamingPlaylists::getSize() {
  return playlists->size();
}

void StreamingPlaylists::addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist) {
  playlists->push_back(std::move(playlist));
}

std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> StreamingPlaylists::getPlaylists() {
  return playlists;
}

std::string StreamingPlaylists::getRootVirtualPath() {
  return rootVirtualPath;
}
