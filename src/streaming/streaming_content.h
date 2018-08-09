/*GRB*
  Gerbera - https://gerbera.io/

  streaming_content.h - this file is part of Gerbera.

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

/// \file streaming_content.h

#ifndef GERBERA_STREAMING_CONTENT_H
#define GERBERA_STREAMING_CONTENT_H

#include "inmemory_playlist.h"
#include "playlist_parse_result.h"
class StreamingContent {
public:
    virtual ~StreamingContent() = default;
    virtual void processConfiguredPlaylists() = 0;
    virtual void printUrl(std::string url) = 0;
    virtual std::shared_ptr<InMemoryPlaylist> downloadPlaylist(std::string name, std::string url) = 0;
    virtual std::shared_ptr<PlaylistParseResult> parsePlaylist(std::shared_ptr<InMemoryPlaylist> playlist) = 0;
    virtual unsigned long persistPlaylist(std::shared_ptr<PlaylistParseResult> parseResult) = 0;
};

#endif //GERBERA_STREAMING_CONTENT_H
