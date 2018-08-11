/*GRB*
  Gerbera - https://gerbera.io/

  test_curldownloader.cc - this file is part of Gerbera.

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

/// \file test_curldownloader.cc

#ifdef HAVE_CURL

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <streaming/curl_downloader.h>

class CurlDownloaderTest : public ::testing::Test {

 public:

  CurlDownloaderTest() {};

  virtual ~CurlDownloaderTest() {};

  virtual void SetUp() {
    subject = std::make_unique<CurlDownloader>();
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::unique_ptr<CurlDownloader> subject;
};

#endif //HAVE_CURL