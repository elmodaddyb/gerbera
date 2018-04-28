#include <streaming/streaming_playlists.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;

class StreamingPlaylistsTest : public ::testing::Test {

 public:

  StreamingPlaylistsTest() {};

  virtual ~StreamingPlaylistsTest() {};

  virtual void SetUp() {
    subject = new StreamingPlaylists();
  }
  virtual void TearDown() {}

  StreamingPlaylists *subject;
};

TEST_F(StreamingPlaylistsTest, CreatesStreamingPlaylistWithEmptyList) {
  ASSERT_EQ(0, subject->getSize());
}

TEST_F(StreamingPlaylistsTest, AllowsToAddRemotePlaylists) {
  auto playlist = make_shared<RemotePlaylist>();
  playlist->setUrl("http://localhost");

  subject->addPlaylist(playlist);

  ASSERT_EQ(1, subject->getSize());
}

TEST_F(StreamingPlaylistsTest, ProvidesListOfAllPlaylists) {
  auto playlist = make_shared<RemotePlaylist>();
  playlist->setUrl("http://localhost");
  subject->addPlaylist(playlist);

  auto playlists = subject->getPlaylists();

  ASSERT_EQ(1, playlists->size());
  EXPECT_STREQ(playlists->at(0)->getUrl().c_str(), playlist->getUrl().c_str());
}

TEST_F(StreamingPlaylistsTest, AllowsToAddSeveralRemotePlaylists) {
  auto playlist = make_shared<RemotePlaylist>();
  playlist->setUrl("http://localhost/playlist1");
  subject->addPlaylist(playlist);

  playlist = make_shared<RemotePlaylist>();
  playlist->setUrl("http://localhost/playlist2");
  subject->addPlaylist(playlist);

  ASSERT_EQ(2, subject->getSize());
  ASSERT_STREQ("http://localhost/playlist1", subject->getPlaylists()->at(0)->getUrl().c_str());
  ASSERT_STREQ("http://localhost/playlist2", subject->getPlaylists()->at(1)->getUrl().c_str());
}
