#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <streaming/playlist_task.h>

#include "streaming/remote_playlist.h"
#include "mock/task_threadpool_mock.h"

using namespace zmm;
using namespace mxml;

class StreamingServiceMock : public StreamingContent {
public:
    MOCK_METHOD0(processConfiguredPlaylists, void());
    MOCK_METHOD1(printUrl, void(std::string url));
};


class PlaylistTaskTest : public ::testing::Test {

 public:

  PlaylistTaskTest() {};

  virtual ~PlaylistTaskTest() {};

  virtual void SetUp() {
    streamingService.reset(new ::testing::NiceMock<StreamingServiceMock>());
    subject = std::make_unique<PlaylistTask>("http://localhost/playlist", streamingService.get());
  }
  virtual void TearDown() {
    subject = nullptr;
  }

  std::string mockPlaylist(std::string mockFile) {
    std::ifstream t(mockFile);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
  }

  zmm::Ref<Element> mockConfig(std::string enabledShoutcast) {
    zmm::Ref<Element> streaming(new Element(_("streaming")));

    zmm::Ref<Element> playlists(new Element(_("playlists")));
    streaming->appendElementChild(playlists);

    zmm::Ref<Element> playlist(new Element(_("playlist")));
    playlist->setAttribute(_("url"), _("http://localhost/playlist"));
    playlists->appendElementChild(playlist);

    zmm::Ref<Element> playlist2(new Element(_("playlist")));
    playlist2->setAttribute(_("url"), _("http://localhost/playlist2"));
    playlists->appendElementChild(playlist2);

    zmm::Ref<Element> shoutcast(new Element(_("shoutcast")));
    shoutcast->setAttribute(_("base-url"), _("http://api.shoutcast.com"));
    shoutcast->setAttribute(_("dev-id"), _("ABC123"));
    shoutcast->setAttribute(_("enabled"), String::copy(enabledShoutcast.c_str()));
    streaming->appendElementChild(shoutcast);
    return streaming;
  }

  std::unique_ptr<PlaylistTask> subject;
  std::shared_ptr<StreamingOptions> streamingOptionsMock;
  std::shared_ptr<TaskThreadPoolMock> threadPoolMock;
  std::shared_ptr<StreamingServiceMock> streamingService;
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

