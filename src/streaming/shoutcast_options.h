/*GRB*
  Gerbera - https://gerbera.io/

  shoutcast_options.h - this file is part of Gerbera.

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

/// \file shoutcast_options.h

#ifndef GERBERA_SHOUTCAST_OPTIONS_H
#define GERBERA_SHOUTCAST_OPTIONS_H

#include <string>
class ShoutcastOptions {
 public:
  ShoutcastOptions(std::string baseUrl, std::string devId, bool enabled);
  ~ShoutcastOptions();
  std::string getBaseUrl();
  std::string getDevId();
  bool isEnabled();
 private:
  std::string baseUrl;
  std::string devId;
  bool enabled = false;
};

#endif //GERBERA_SHOUTCAST_OPTIONS_H
