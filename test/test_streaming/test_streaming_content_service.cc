#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <streaming/streaming_content_service.h>
#include <config/IConfigManager.h>
#include <zmm/zmm.h>
#include <mxml/mxml.h>
#include <task/task_threadpool.h>

#include "streaming/remote_playlist.h"

using namespace zmm;
using namespace mxml;
using namespace ::testing;

class ConfigManagerMock : public IConfigManager {
public:
    MOCK_METHOD0(getStreamingOptions, std::shared_ptr<StreamingOptions>());
};

class TaskThreadPoolMock : public ThreadPool {
public:
    MOCK_METHOD0(getPoolSize, long long());
    MOCK_METHOD0(getQueueSize, long long());
    MOCK_METHOD0(tasksReceived, long long());
    MOCK_METHOD0(tasksCompleted, long long());
    MOCK_METHOD1(enqueue, void(std::shared_ptr<Task> t));
    MOCK_METHOD1(start, void(int numberOfThreads));
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(isShutdown, bool());
};

class StreamingContentServiceTest : public ::testing::Test {

public:

    StreamingContentServiceTest() {};

    virtual ~StreamingContentServiceTest() {};

    virtual void SetUp() {
      streamingOptions = std::make_shared<StreamingOptions>(mockConfig("yes"));
      taskThreadPoolMock.reset(new ::testing::NiceMock<TaskThreadPoolMock>());
      subject = std::make_unique<StreamingContentService>(streamingOptions, taskThreadPoolMock);
    }

    virtual void TearDown() {
      streamingOptions = nullptr;
      taskThreadPoolMock = nullptr;
      subject = nullptr;
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

    std::unique_ptr<StreamingContentService> subject;
    std::shared_ptr<StreamingOptions> streamingOptions;
    std::shared_ptr<TaskThreadPoolMock> taskThreadPoolMock;
};

TEST_F(StreamingContentServiceTest, CreateStreamingContentService) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(StreamingContentServiceTest, RetrievesTheConfiguredPlaylistsAndQueuesTasksToProcess) {
  EXPECT_CALL(*taskThreadPoolMock, enqueue(_)).Times(2).WillRepeatedly(Return());

  subject->processConfiguredPlaylists();
}

