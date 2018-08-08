/*GRB*
  Gerbera - https://gerbera.io/

  abstract_storage.h - this file is part of Gerbera.

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

/// \file abstract_storage.h

#ifndef GERBERA_ABSTRACT_STORAGE_H
#define GERBERA_ABSTRACT_STORAGE_H

#include <cds_objects.h>
class AbstractStorage {

 public:
  virtual ~AbstractStorage() {}
  virtual zmm::Ref<CdsObject> loadObject(int objectID) = 0;
  virtual zmm::Ref<CdsObject> findVirtualObjectByPath(zmm::String fullpath) = 0;
};

#endif //GERBERA_ABSTRACT_STORAGE_H
