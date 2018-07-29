/*GRB*
  Gerbera - https://gerbera.io/

  curl_downloader_mock.h - this file is part of Gerbera.

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

/// \file curl_downloader_mock.h

#ifndef GERBERA_CURLDOWNLOADERMOCK_H
#define GERBERA_CURLDOWNLOADERMOCK_H
#include <streaming/curl_downloader.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class CurlDownloaderMock : public CurlDownloader {
  public:
      MOCK_METHOD1(download, std::string(std::string url));
};

#endif //GERBERA_CURLDOWNLOADERMOCK_H
