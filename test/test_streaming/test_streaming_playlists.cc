#include <streaming/streaming_playlists.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

using namespace std;

class StreamingPlaylistsTest : public ::testing::Test {

 public:

  StreamingPlaylistsTest() {};

  virtual ~StreamingPlaylistsTest() {};

  virtual void SetUp() {
    std::string rootVirtualPath = "/Root Virtual Path";
    subject = make_unique<StreamingPlaylists>(rootVirtualPath);
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::unique_ptr<StreamingPlaylists> subject;
};

TEST_F(StreamingPlaylistsTest, CreatesStreamingPlaylistWithEmptyList) {
  ASSERT_EQ(0, subject->getSize());
}

TEST_F(StreamingPlaylistsTest, AllowsToAddRemotePlaylists) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost", "Name of Playlist");

  subject->addPlaylist(std::move(playlist));

  ASSERT_EQ(1, subject->getSize());
}

TEST_F(StreamingPlaylistsTest, ProvidesListOfAllPlaylists) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost", "Name of Playlist");
  subject->addPlaylist(std::move(playlist));

  auto playlists = subject->getPlaylists();

  ASSERT_EQ(1, playlists->size());
  EXPECT_STREQ("http://localhost", playlists->at(0)->getUrl().c_str());
}

TEST_F(StreamingPlaylistsTest, AllowsToAddSeveralRemotePlaylists) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost/playlist1", "Name of Playlist");
  subject->addPlaylist(std::move(playlist));

  playlist = make_unique<ConfiguredPlaylist>("http://localhost/playlist2", "Name of Playlist");
  subject->addPlaylist(std::move(playlist));

  ASSERT_EQ(2, subject->getSize());
  ASSERT_STREQ("http://localhost/playlist1", subject->getPlaylists()->at(0)->getUrl().c_str());
  ASSERT_STREQ("http://localhost/playlist2", subject->getPlaylists()->at(1)->getUrl().c_str());
}

TEST_F(StreamingPlaylistsTest, ContainsVirtualPathForAllPlaylists) {
 std::string result = subject->getRootVirtualPath();
 ASSERT_STREQ(result.c_str(), "/Root Virtual Path");
}

TEST_F(StreamingPlaylistsTest, EachPlaylistHasName) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost/playlist1", "Name of Playlist");
  subject->addPlaylist(std::move(playlist));

  std::string result = subject->getPlaylists()->at(0)->getName();

  ASSERT_STREQ(result.c_str(), "Name of Playlist");
}