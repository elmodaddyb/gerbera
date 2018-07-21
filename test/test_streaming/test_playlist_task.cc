#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <streaming/playlist_task.h>

#include "streaming/remote_playlist.h"

class PlaylistTaskTest : public ::testing::Test {

 public:

  PlaylistTaskTest() {};

  virtual ~PlaylistTaskTest() {};

  virtual void SetUp() {
    subject = std::make_unique<PlaylistTask>("http://localhost/playlist");
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::string mockPlaylist(std::string mockFile) {
    std::ifstream t(mockFile);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
  }

  std::unique_ptr<PlaylistTask> subject;
};

TEST_F(PlaylistTaskTest, CreateRemotePlaylistObject) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(PlaylistTaskTest, HoldsRemotePlaylistContent) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");

  subject->setContent(playlistContent);

  ASSERT_STREQ(playlistContent.c_str(), subject->getContent().c_str());
}

TEST_F(PlaylistTaskTest, ProvidesContentAsVector) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::vector<std::string> result = subject->getContentVector();

  ASSERT_EQ(result.size(), 18);
}

TEST_F(PlaylistTaskTest, ReadsContentFirstLine) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::string result = subject->readLine();

  ASSERT_EQ(result, "[playlist]");
}

TEST_F(PlaylistTaskTest, ReadsContentLineByLine) {
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

TEST_F(PlaylistTaskTest, ThrowsOutOfRangeWhenEmptyPlaylist) {
  subject->setContent("");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}

TEST_F(PlaylistTaskTest, ThrowsOutOfRangeWhenEndPlaylist) {
  subject->setContent("[playlist]\n");
  std::string result = subject->readLine();
  ASSERT_STREQ(result.c_str(), "[playlist]");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}

