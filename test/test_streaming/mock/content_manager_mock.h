/*GRB*
  Gerbera - https://gerbera.io/

  content_manager_mock.h - this file is part of Gerbera.

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

/// \file content_manager_mock.h


#ifndef GERBERA_CONTENT_MANAGER_MOCK_H
#define GERBERA_CONTENT_MANAGER_MOCK_H
#include <content/gerbera_content_manager.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class ContentManagerMock : public GerberaContentManager {
 public:
  MOCK_METHOD3(addContainer, int(int parentID, const zmm::String title, const zmm::String upnpClass));
  MOCK_METHOD1(addObject, void(zmm::Ref<CdsObject> obj));
  MOCK_METHOD4(addContainerChain, int(zmm::String chain, zmm::String lastClass, int lastRefID, zmm::Ref<Dictionary> lastMetadata));
  MOCK_METHOD3(removeObject, void(int objectID, bool async, bool all));
};

#endif //GERBERA_CONTENT_MANAGER_MOCK_H
