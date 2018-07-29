/*GRB*
  Gerbera - https://gerbera.io/

  test_inmemory_playlist.cc - this file is part of Gerbera.

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

/// \file test_inmemory_playlist.cc

#include <streaming/inmemory_playlist.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>

class InMemoryPlaylistTest : public ::testing::Test {

 public:

  InMemoryPlaylistTest() {};

  virtual ~InMemoryPlaylistTest() {};

  virtual void SetUp() {
    subject = std::make_unique<InMemoryPlaylist>("http://localhost/playlist");
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::string mockPlaylist(std::string mockFile) {
    std::ifstream t(mockFile);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
  }

  std::unique_ptr<InMemoryPlaylist> subject;
};

TEST_F(InMemoryPlaylistTest, HoldsRemotePlaylistContent) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");

  subject->setContent(playlistContent);

  ASSERT_STREQ(playlistContent.c_str(), subject->getContent().c_str());
}

TEST_F(InMemoryPlaylistTest, ProvidesContentAsVector) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::vector<std::string> result = subject->getContentVector();

  ASSERT_EQ(result.size(), 18);
}

TEST_F(InMemoryPlaylistTest, ReadsContentFirstLine) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::string result = subject->readLine();

  ASSERT_EQ(result, "[playlist]");
}

TEST_F(InMemoryPlaylistTest, ReadsContentLineByLine) {
  std::string playlistContent = mockPlaylist("fixtures/simple.pls");
  subject->setContent(playlistContent);
  std::vector<std::string> expected = {
      "[playlist]",
      "",
      "File1=http://relay5.181.fm:8068",
      "Length1=-1",
      "",
      "File2=example2.mp3",
      "Title2=Just some local audio that is 2mins long",
      "Length2=120",
      "",
      "NumberOfEntries=2",
      "Version=2"
  };

  for(auto const& value: expected) {
    std::string result = subject->readLine();
    ASSERT_STREQ(result.c_str(), value.c_str());
  }
}

TEST_F(InMemoryPlaylistTest, ThrowsOutOfRangeWhenEmptyPlaylist) {
  subject->setContent("");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}

TEST_F(InMemoryPlaylistTest, ThrowsOutOfRangeWhenEndPlaylist) {
  subject->setContent("[playlist]\n");
  std::string result = subject->readLine();
  ASSERT_STREQ(result.c_str(), "[playlist]");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}