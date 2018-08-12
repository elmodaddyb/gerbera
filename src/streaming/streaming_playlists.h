/*GRB*
  Gerbera - https://gerbera.io/

  streaming_playlists.h - this file is part of Gerbera.

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

/// \file streaming_playlists.h

#ifndef GERBERA_STREAMING_PLAYLISTS_H
#define GERBERA_STREAMING_PLAYLISTS_H

#include <vector>
#include "configured_playlist.h"
#include <memory>

class StreamingPlaylists {
 public:
  StreamingPlaylists(std::string rootVirtualPath);
  ~StreamingPlaylists();
  unsigned long getSize();
  void addPlaylist(std::unique_ptr<ConfiguredPlaylist> playlist);
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> getPlaylists();
  std::string getRootVirtualPath();

 private:
  std::shared_ptr<std::vector<std::unique_ptr<ConfiguredPlaylist>>> playlists;
  std::string rootVirtualPath;
};

#endif //GERBERA_STREAMING_PLAYLISTS_H
