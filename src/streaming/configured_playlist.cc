/*GRB*
  Gerbera - https://gerbera.io/

  configured_playlist.cc - this file is part of Gerbera.

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

/// \file configured_playlist.cc

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "configured_playlist.h"

ConfiguredPlaylist::ConfiguredPlaylist(std::string url, std::string name) : url(std::move(url)), name(std::move(name)) {}

std::string ConfiguredPlaylist::getUrl() {
  return this->url;
}

std::string ConfiguredPlaylist::getName() {
  return this->name;
}