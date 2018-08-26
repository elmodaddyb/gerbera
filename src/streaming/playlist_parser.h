/*GRB*
  Gerbera - https://gerbera.io/

  playlist_parser.h - this file is part of Gerbera.

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

/// \file playlist_parser.h

#ifndef GERBERA_PLAYLIST_PARSER_H
#define GERBERA_PLAYLIST_PARSER_H

#include <cds_objects.h>
#include <memory>
#include <streaming/inmemory_playlist.h>
#include <zmm/zmmf.h>

#define REGEX_PLAYLIST  "^\\[playlist\\]$"
#define REGEX_PLS_FILE  "^File(\\d)+?=(.*)$"
#define REGEX_PLS_TITLE "^Title(\\d)+?=(.*)$"
#define REGEX_M3U       "^#EXTM3U$"
#define REGEX_M3U_TITLE "^(#EXTINF:)([-]?[\\d]+),(.*)$"
#define REGEX_XSPF_XML  ".*xmlns=\"http://xspf.org/ns/0/\".*"
#define XSPF_TRACKLIST  "/trackList"
#define XSPF_LOCATION   "location"
#define XSPF_TITLE      "title"

enum PlaylistType {
    PLS, M3U, XSPF, UNKNOWN
};

class PlaylistParser {
public:
    PlaylistParser() = default;
    ~PlaylistParser() = default;
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parsePlaylist(std::shared_ptr<InMemoryPlaylist>& playlist);
private:
    PlaylistType determinePlaylistType(std::string firstLine);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist);
    std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> parseXSPF(std::shared_ptr<InMemoryPlaylist>& playlist);
    zmm::Ref<CdsItemExternalURL> createExternalUrl(std::string title, std::string location);
};


#endif //GERBERA_PLAYLIST_PARSER_H
