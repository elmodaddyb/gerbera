/*GRB*
  Gerbera - https://gerbera.io/

  test_url.cc - this file is part of Gerbera.

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

/// \file test_url.cc

#ifdef HAVE_CURL

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <gerbera/gerbera.h>
#include <gerbera/url.h>

using namespace ::testing;
using namespace gerbera;

class URLTest : public ::testing::Test {

 public:

  URLTest() {};

  virtual ~URLTest() {};

  virtual void SetUp() {
    subject = std::make_unique<URL>("http://localhost/url");
  }

  virtual void TearDown() {
    subject = nullptr;
  }

  std::unique_ptr<URL> subject;
};

TEST_F(URLTest, InstantiatesURL) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(URLTest, SupportsStatInfoFromURL) {
  off_t size = 200;
  auto stat = std::make_unique<URL::Stat>("http://localhost", size, "text/html");

  EXPECT_NE(stat, nullptr);
}

#endif //HAVE_CURL


