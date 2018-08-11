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
#ifdef HAVE_CURL

#ifndef GERBERA_CURL_DOWNLOADER_MOCK_H
#define GERBERA_CURL_DOWNLOADER_MOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <gerbera/url.h>
#include <streaming/downloader.h>

using namespace ::testing;

class CurlDownloaderMock : public Downloader {
 public:
  MOCK_METHOD1(download, std::string(const std::shared_ptr<gerbera::URL>& url));
};


#endif //GERBERA_CURL_DOWNLOADER_MOCK_H
#endif //HAVE_CURL