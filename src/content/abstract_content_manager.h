/*GRB*
  Gerbera - https://gerbera.io/

  abstract_content_manager.h - this file is part of Gerbera.

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

/// \file abstract_content_manager.cc

#ifndef GERBERA_ICONTENTMANAGER_H
#define GERBERA_ICONTENTMANAGER_H

#include "zmm/zmm.h"
#include "cds_objects.h"

class AbstractContentManager {

 public:
  virtual ~AbstractContentManager() {}
  virtual int addContainer(int parentID, zmm::String title, zmm::String upnpClass) = 0;
  virtual void addObject(zmm::Ref<CdsObject> obj) = 0;
  virtual int addContainerChain(zmm::String chain, zmm::String lastClass, int lastRefID, zmm::Ref<Dictionary> lastMetadata) = 0;
};


#endif //GERBERA_ICONTENTMANAGER_H
