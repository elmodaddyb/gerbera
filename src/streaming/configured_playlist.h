/*GRB*
  Gerbera - https://gerbera.io/

  configured_playlist.h - this file is part of Gerbera.

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

/// \file configured_playlist.h

#ifndef GERBERA_REMOTE_PLAYLIST_H
#define GERBERA_REMOTE_PLAYLIST_H

#include <string>

class ConfiguredPlaylist {
 public:
  ConfiguredPlaylist() = default;
  explicit ConfiguredPlaylist(std::string url, std::string name);
  virtual ~ConfiguredPlaylist() = default;
  std::string getUrl();
  std::string getName();
 private:
  std::string url;
  std::string name;
};

#endif //GERBERA_REMOTE_PLAYLIST_H
