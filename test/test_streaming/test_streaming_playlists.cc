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
    subject = make_unique<StreamingPlaylists>();
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
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost");

  subject->addPlaylist(std::move(playlist));

  ASSERT_EQ(1, subject->getSize());
}

TEST_F(StreamingPlaylistsTest, ProvidesListOfAllPlaylists) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost");
  subject->addPlaylist(std::move(playlist));

  auto playlists = subject->getPlaylists();

  ASSERT_EQ(1, playlists->size());
  EXPECT_STREQ("http://localhost", playlists->at(0)->getUrl().c_str());
}

TEST_F(StreamingPlaylistsTest, AllowsToAddSeveralRemotePlaylists) {
  auto playlist = make_unique<ConfiguredPlaylist>("http://localhost/playlist1");
  subject->addPlaylist(std::move(playlist));

  playlist = make_unique<ConfiguredPlaylist>("http://localhost/playlist2");
  subject->addPlaylist(std::move(playlist));

  ASSERT_EQ(2, subject->getSize());
  ASSERT_STREQ("http://localhost/playlist1", subject->getPlaylists()->at(0)->getUrl().c_str());
  ASSERT_STREQ("http://localhost/playlist2", subject->getPlaylists()->at(1)->getUrl().c_str());
}
