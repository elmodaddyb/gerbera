/*GRB*
  Gerbera - https://gerbera.io/

  playlist_parse_result.h - this file is part of Gerbera.

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

/// \file playlist_parse_result.h

#ifndef GERBERA_PLAYLISTPARSERESULT_H
#define GERBERA_PLAYLISTPARSERESULT_H

#include <cds_objects.h>

class PlaylistParseResult {
 public:
  explicit PlaylistParseResult(std::shared_ptr<CdsContainer> parent);
  ~PlaylistParseResult();
  std::shared_ptr<CdsContainer> getParentContainer();
  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> getChildObjects();
  void addItem(zmm::Ref<CdsItemExternalURL> cdsObject);

 private:
  std::shared_ptr<CdsContainer> parentContainer;
  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> childObjects;

};

#endif //GERBERA_PLAYLISTPARSERESULT_H
