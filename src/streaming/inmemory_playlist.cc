/*GRB*
  Gerbera - https://gerbera.io/

  inmemory_playlist.cc - this file is part of Gerbera.

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

/// \file inmemory_playlist.cc

#include <string>
#include <sstream>
#include <stdexcept>
#include "inmemory_playlist.h"

InMemoryPlaylist::InMemoryPlaylist(std::string content) {
  this->setContent(std::move(content));
}

void InMemoryPlaylist::setContent(std::string content) {
  this->content.assign(content);
  this->contentVector.clear();
  std::istringstream f(content);
  std::string line;
  while (std::getline(f, line)) {
    contentVector.push_back(line);
  }
}

std::string InMemoryPlaylist::getContent() {
  return this->content;
}

std::vector<std::string> InMemoryPlaylist::getContentVector() {
  return this->contentVector;
}

std::string InMemoryPlaylist::readLine() {
  if(!contentVector.empty() && contentVector.size() >= lineNumber) {
    return contentVector.at(lineNumber++);
  } else {
    throw std::out_of_range ("End of playlist reached");
  }
}
