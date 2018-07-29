/*GRB*
  Gerbera - https://gerbera.io/

  playlist_parse_result.cc - this file is part of Gerbera.

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

/// \file playlist_parse_result.cc

#include <memory>
#include <vector>
#include "playlist_parse_result.h"

PlaylistParseResult::PlaylistParseResult(std::shared_ptr<CdsContainer> parent) : parentContainer(std::move(parent)) {
  childObjects = std::make_shared<std::vector<zmm::Ref<CdsItemExternalURL>>>();
}

PlaylistParseResult::~PlaylistParseResult() {
  parentContainer = nullptr;
  childObjects = nullptr;
}

void PlaylistParseResult::addItem(zmm::Ref<CdsItemExternalURL> cdsObject) {
  childObjects->push_back(std::move(cdsObject));
}

std::shared_ptr<CdsContainer> PlaylistParseResult::getParentContainer() {
  return parentContainer;
}

std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> PlaylistParseResult::getChildObjects() {
  return childObjects;
}