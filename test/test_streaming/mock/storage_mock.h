/*GRB*
  Gerbera - https://gerbera.io/

  storage_mock.h - this file is part of Gerbera.

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

/// \file storage_mock.h

#ifndef GERBERA_STORAGE_MOCK_H
#define GERBERA_STORAGE_MOCK_H
#include <storage/gerbera_storage.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class StorageMock : public GerberaStorage {
 public:
    MOCK_METHOD1(loadObject, zmm::Ref<CdsObject>(int objectID));
    MOCK_METHOD1(findVirtualObjectByPath, zmm::Ref<CdsObject>(zmm::String fullpath));
};

#endif //GERBERA_STORAGE_MOCK_H
