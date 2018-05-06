#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>

#include "streaming/remote_playlist.h"

class RemotePlaylistTest : public ::testing::Test {

 public:

  RemotePlaylistTest() {};

  virtual ~RemotePlaylistTest() {};

  virtual void SetUp() {
    subject = std::make_unique<RemotePlaylist>();
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::string mockPlaylist(std::string mockFile) {
    std::ifstream t(mockFile);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
  }

  std::unique_ptr<RemotePlaylist> subject;
};

TEST_F(RemotePlaylistTest, CreateRemotePlaylistObject) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(RemotePlaylistTest, ProvidesTheRemoteUrlOfPlaylist) {
  std::string expectedUrl = "http://localhost/playlist.pls";
  subject->setUrl(expectedUrl);

  ASSERT_STREQ(subject->getUrl().c_str(), expectedUrl.c_str());
}

TEST_F(RemotePlaylistTest, HoldsRemotePlaylistContent) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");

  subject->setContent(playlistContent);

  ASSERT_STREQ(playlistContent.c_str(), subject->getContent().c_str());
}

TEST_F(RemotePlaylistTest, ProvidesContentAsVector) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::vector<std::string> result = subject->getContentVector();

  ASSERT_EQ(result.size(), 18);
}

TEST_F(RemotePlaylistTest, ReadsContentFirstLine) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  subject->setContent(playlistContent);

  std::string result = subject->readLine();

  ASSERT_EQ(result, "[playlist]");
}

TEST_F(RemotePlaylistTest, ReadsContentLineByLine) {
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

TEST_F(RemotePlaylistTest, ThrowsOutOfRangeWhenEmptyPlaylist) {
  subject->setContent("");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}

TEST_F(RemotePlaylistTest, ThrowsOutOfRangeWhenEndPlaylist) {
  subject->setContent("[playlist]\n");
  std::string result = subject->readLine();
  ASSERT_STREQ(result.c_str(), "[playlist]");
  EXPECT_THROW(subject->readLine(), std::out_of_range);
}

