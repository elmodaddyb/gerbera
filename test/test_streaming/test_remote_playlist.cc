#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "streaming/remote_playlist.h"

class RemotePlaylistTest : public ::testing::Test {

 public:

  RemotePlaylistTest() {};

  virtual ~RemotePlaylistTest() {};

  virtual void SetUp() {
    subject = new RemotePlaylist();
  }
  virtual void TearDown() {}

  RemotePlaylist *subject;
};

TEST_F(RemotePlaylistTest, CreateRemotePlaylistObject) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(RemotePlaylistTest, ProvidesTheRemoteUrlOfPlaylist) {
  std::string expectedUrl = "http://localhost/playlist.pls";
  subject->setUrl(expectedUrl);

  ASSERT_STREQ(subject->getUrl().c_str(), expectedUrl.c_str());
}

