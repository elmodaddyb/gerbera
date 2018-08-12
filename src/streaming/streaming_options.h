/*GRB*
  Gerbera - https://gerbera.io/

  streaming_options.h - this file is part of Gerbera.

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

/// \file streaming_options.h

#ifndef GERBERA_STREAMING_OPTIONS_H
#define GERBERA_STREAMING_OPTIONS_H

#include <mxml/mxml.h>
#include "streaming_playlists.h"
#include "shoutcast_options.h"
using namespace mxml;

class StreamingOptions {
 public:
  StreamingOptions(zmm::Ref<Element> streamingElement);
  ~StreamingOptions();
  std::shared_ptr<StreamingPlaylists> getPlaylists();
  std::shared_ptr<ShoutcastOptions> getShoutcastOptions();

 private:
  std::shared_ptr<StreamingPlaylists> playlists;
  std::shared_ptr<ShoutcastOptions> shoutcast;
};

#endif //GERBERA_STREAMING_OPTIONS_H
