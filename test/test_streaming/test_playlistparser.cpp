#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <string>
#include <streaming/inmemory_playlist.h>
#include <streaming/playlist_parser.h>
#include <zmm/zmmf.h>
#include <cds_objects.h>

class PlaylistParserTest : public ::testing::Test {

public:
    PlaylistParserTest() = default;
    ~PlaylistParserTest() override = default;
    void SetUp() override {
      subject = std::make_unique<PlaylistParser>();
    }
    void TearDown() override {
      subject = nullptr;
    }

    std::shared_ptr<InMemoryPlaylist> mockPlaylist(std::string& mockFile) {
      std::ifstream t(mockFile);
      std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

      return std::make_shared<InMemoryPlaylist>("playlist", str);
    }

    std::unique_ptr<PlaylistParser> subject;
};

TEST_F(PlaylistParserTest, CreatePlaylistParserObject) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(PlaylistParserTest, CreatesExternalURLsForM3U) {
  std::string fixture = "fixtures/example.m3u";
  std::shared_ptr<InMemoryPlaylist> playlist = mockPlaylist(fixture);

  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> items = subject->parsePlaylist(playlist);

  ASSERT_EQ(items->size(), 8);
  ASSERT_STREQ(items->at(0)->getTitle().c_str(), "(#1 - 49/750) : Ibiza Global Radio :");
  ASSERT_STREQ(items->at(0)->getURL().c_str(), "http://46.105.171.217:8024");
}

TEST_F(PlaylistParserTest, CreatesExternalURLsForPLS) {
  std::string fixture = "fixtures/example.pls";
  std::shared_ptr<InMemoryPlaylist> playlist = mockPlaylist(fixture);

  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> items = subject->parsePlaylist(playlist);

  ASSERT_EQ(items->size(), 5);
  ASSERT_STREQ(items->at(0)->getTitle().c_str(), "(#1 - 28/500) Classic Rock 109");
  ASSERT_STREQ(items->at(0)->getURL().c_str(), "http://107.191.38.213:10042/stream");
}

TEST_F(PlaylistParserTest, CreatesExternalURLsForXSPF) {
  std::string fixture = "fixtures/example.xspf";
  std::shared_ptr<InMemoryPlaylist> playlist = mockPlaylist(fixture);

  std::shared_ptr<std::vector<zmm::Ref<CdsItemExternalURL>>> items = subject->parsePlaylist(playlist);

  ASSERT_EQ(items->size(), 2);
  ASSERT_STREQ(items->at(0)->getTitle().c_str(), "Dance Wave!");
  ASSERT_STREQ(items->at(0)->getURL().c_str(), "http://78.31.65.20:8080/dance.mp3");
}

