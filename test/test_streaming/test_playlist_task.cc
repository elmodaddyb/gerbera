#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <streaming/playlist_task.h>
#include <streaming/playlist_parse_result.h>

#include "streaming/configured_playlist.h"
#include "mock/task_threadpool_mock.h"
#include "mock/streaming_service_mock.h"

using namespace zmm;
using namespace mxml;

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

  std::unique_ptr<PlaylistTask> subject;
  std::shared_ptr<StreamingServiceMock> streamingService;
};

TEST_F(PlaylistTaskTest, CreateRemotePlaylistObject) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(PlaylistTaskTest, CallStreamingServiceToProcessPlaylist) {
  std::shared_ptr<InMemoryPlaylist> inMemoryPlaylist = std::make_shared<InMemoryPlaylist>("content");
  std::shared_ptr<CdsContainer> parentCds = std::make_shared<CdsContainer>();
  parentCds->setTitle("Playlist Task Test Playlist");
  std::shared_ptr<PlaylistParseResult> parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  unsigned long expItemsAdded = 13;

  EXPECT_CALL(*streamingService, downloadPlaylist(Eq("http://localhost/playlist"))).WillOnce(Return(inMemoryPlaylist));
  EXPECT_CALL(*streamingService, parsePlaylist(_)).WillOnce(Return(parseResult));
  EXPECT_CALL(*streamingService, persistPlaylist(_)).WillOnce(Return(expItemsAdded));

  subject->run();
}


