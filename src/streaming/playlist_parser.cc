/*GRB*
  Gerbera - https://gerbera.io/

  playlist_parser.cc - this file is part of Gerbera.

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

/// \file playlist_parser.cc

#include <memory>
#include <metadata_handler.h>
#include <online_service.h>
#include <regex>
#include <xpath.h>
#include <tools.h>

#include "playlist_parser.h"

using namespace zmm;
using namespace mxml;

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> PlaylistParser::parsePlaylist(std::shared_ptr<InMemoryPlaylist>& playlist) {
  std::string firstLine = playlist->getContentVector().at(0);
  PlaylistType type = determinePlaylistType(firstLine);
  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> playlistItems;
  switch(type) {
    case PLS: {
      playlistItems = parsePLS(playlist);
      break;
    }
    case M3U: {
      playlistItems = parseM3U(playlist);
      break;
    }
    case XSPF: {
      playlistItems = parseXSPF(playlist);
      break;
    }
    default: {
      log_error("Unable to identify playlist type from line: \n%s\n", firstLine.c_str());
      playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
    }
  }

  log_debug("Parsed %d playlist items with playlist type: %d\n", playlistItems->size(), type);
  return playlistItems;
}

PlaylistType PlaylistParser::determinePlaylistType(std::string firstLine) {
  PlaylistType type;
  std::regex plsRegex(REGEX_PLAYLIST);
  std::regex m3uRegex(REGEX_M3U);
  std::regex xspfRegex(REGEX_XSPF_XML);
  if(std::regex_match(firstLine.c_str(), plsRegex)) {
    type = PLS;
  } else if (std::regex_match(firstLine.c_str(), m3uRegex)) {
    type = M3U;
  } else if (std::regex_match(firstLine.c_str(), xspfRegex)) {
    type = XSPF;
  } else {
    type = UNKNOWN;
  }
  log_debug("Playlist type: %d\n", type);
  return type;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> PlaylistParser::parsePLS(std::shared_ptr<InMemoryPlaylist>& playlist) {
  log_debug("Begin parsing playlist: PLS type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

  std::regex fileRegex(REGEX_PLS_FILE);
  std::regex titleRegex(REGEX_PLS_TITLE);

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, fileRegex)) {
      if(pieces.size() >= 2) {
        std::string location = pieces.str(2);

        // Get the Title
        std::string title;
        line = playlistLines.at(++i);
        if (std::regex_match(line.c_str(), pieces, titleRegex)) {
          title = pieces.str(2);
        } else {
          title = "";
        }

        try {
          auto newObject = createExternalUrl(title, location);
          playlistItems->push_back(newObject);
        } catch (const Exception &ex) {
          log_warning("Failed to validate newly created Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }
  return playlistItems;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> PlaylistParser::parseM3U(std::shared_ptr<InMemoryPlaylist>& playlist) {
  log_debug("Begin parsing playlist: M3U type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistLines = playlist->getContentVector();

  std::regex titleRegex(REGEX_M3U_TITLE);

  for(unsigned long i = 0; i < playlistLines.size(); i++) {
    auto line = playlistLines.at(i);
    std::cmatch pieces;
    if (std::regex_match(line.c_str(), pieces, titleRegex)) {
      if(pieces.size() >= 4) {
        std::string title = pieces.str(3);
        std::string location = playlistLines.at(++i);
        try {
          auto newObject = createExternalUrl(title, location);
          playlistItems->push_back(newObject);
        }
        catch (const Exception &ex) {
          log_warning("Failed to validate newly created Playlist item: %s\n", ex.getMessage().c_str());
          continue;
        }
      }
    }
  }
  return playlistItems;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> PlaylistParser::parseXSPF(std::shared_ptr<InMemoryPlaylist>& playlist) {
  log_debug("Begin parsing playlist: XSPF type received %d bytes\n", playlist->getContent().size());
  auto playlistItems = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
  auto playlistContent = playlist->getContent();

  Ref<Parser> parser(new Parser());
  zmm::Ref<mxml::Document> rootDoc =  parser->parseString(playlistContent);
  zmm::Ref<mxml::Element> root = rootDoc->getRoot();
  Ref<XPath> rootXPath(new XPath(root));
  Ref<Element> trackList = rootXPath->getElement(XSPF_TRACKLIST);

  Ref<Element> track;
  for (int e = 0; e < trackList->elementChildCount(); e++) {
    track = trackList->getElementChild(e);
    Ref<Element> location = track->getChildByName(XSPF_LOCATION);
    Ref<Element> title = track->getChildByName(XSPF_TITLE);
    std::string titleStr = std::string(title->getText().c_str());
    std::string locationStr = std::string(location->getText().c_str());

    try {
      auto newObject = createExternalUrl(titleStr, locationStr);
      playlistItems->push_back(newObject);
    } catch (const Exception &ex) {
      log_warning("Failed to validate newly created Playlist item: %s\n", ex.getMessage().c_str());
      continue;
    }

  }
  return playlistItems;
}

zmm::Ref<CdsItemExternalURL> PlaylistParser::createExternalUrl(std::string title, std::string location) {
  struct timespec ts;
  auto newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  newObject->setTitle(title);
  newObject->setClass(UPNP_DEFAULT_CLASS_MUSIC_TRACK);

  // Add HTTP Protocol
  zmm::Ref<CdsResource> resource(new CdsResource(CH_DEFAULT));
  zmm::String protocolInfo = renderProtocolInfo(MIMETYPE_DEFAULT, PROTOCOL);
  resource->addAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO), protocolInfo);
  newObject->addResource(resource);

  getTimespecNow(&ts);
  newObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), zmm::String::from(ts.tv_sec));
  newObject->setURL(location);
  newObject->setVirtual(true);
  newObject->validate();
  return newObject;
}

