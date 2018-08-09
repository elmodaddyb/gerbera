/*GRB*
  Gerbera - https://gerbera.io/

  streaming_service_mock.h - this file is part of Gerbera.

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

/// \file streaming_service_mock.h

#ifndef GERBERA_STREAMING_CONTENT_SVC_MOCK_H
#define GERBERA_STREAMING_CONTENT_SVC_MOCK_H
#include <streaming/streaming_content.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class StreamingServiceMock : public StreamingContent {
 public:
  MOCK_METHOD0(processConfiguredPlaylists, void());
  MOCK_METHOD1(printUrl, void(std::string url));
  MOCK_METHOD2(downloadPlaylist, std::shared_ptr<InMemoryPlaylist>(std::string name, std::string url));
  MOCK_METHOD1(parsePlaylist, std::shared_ptr<PlaylistParseResult>(std::shared_ptr<InMemoryPlaylist> playlist));
  MOCK_METHOD1(persistPlaylist, unsigned long(std::shared_ptr<PlaylistParseResult> parseResult));
};
#endif //GERBERA_STREAMING_CONTENT_SVC_MOCK_H
