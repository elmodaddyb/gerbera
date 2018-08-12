/*GRB*
  Gerbera - https://gerbera.io/

  shoutcast_options.cc - this file is part of Gerbera.

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

/// \file shoutcast_options.cc

#include "shoutcast_options.h"

ShoutcastOptions::ShoutcastOptions(std::string baseUrl, std::string devId, bool enabled) {
  this->baseUrl = baseUrl;
  this->devId = devId;
  this->enabled = enabled;
}

ShoutcastOptions::~ShoutcastOptions() {}

std::string ShoutcastOptions::getBaseUrl() {
  return baseUrl;
}

std::string ShoutcastOptions::getDevId() {
  return devId;
}

bool ShoutcastOptions::isEnabled() {
  return enabled;
}



