/*GRB*
  Gerbera - https://gerbera.io/

  inmemory_playlist.h - this file is part of Gerbera.

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

/// \file inmemory_playlist.h

#ifndef GERBERA_INMEMORY_PLAYLIST_H
#define GERBERA_INMEMORY_PLAYLIST_H

#include <vector>
#include <string>

class InMemoryPlaylist {
 public:
  InMemoryPlaylist(std::string name, std::string content);
  ~InMemoryPlaylist() = default;
  void setContent(std::string content);
  std::string getContent();
  std::string getName();
  std::string readLine();
  std::vector<std::string> getContentVector();
 private:
  std::string name;
  std::string content;
  std::vector<std::string> contentVector;
  unsigned long lineNumber = 0;
};

#endif //GERBERA_INMEMORY_PLAYLIST_H
