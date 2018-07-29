#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include <fstream>
#include <streaming/streaming_content_service.h>
#include <zmm/zmm.h>
#include <mxml/mxml.h>
#include <task/task_threadpool.h>
#include <metadata_handler.h>
#include <online_service.h>

#include "streaming/configured_playlist.h"
#include "mock/task_threadpool_mock.h"
#include "mock/curl_downloader_mock.h"
#include "mock/content_manager_mock.h"
#include "mock/storage_mock.h"

using namespace zmm;
using namespace mxml;
using namespace ::testing;

class StreamingContentServiceTest : public ::testing::Test {

public:

    StreamingContentServiceTest() {};

    virtual ~StreamingContentServiceTest() {};

    virtual void SetUp() {
      streamingOptions = std::make_shared<StreamingOptions>(mockConfig("yes"));
      taskThreadPoolMock.reset(new ::testing::NiceMock<TaskThreadPoolMock>());
      curlDownloaderMock.reset(new ::testing::NiceMock<CurlDownloaderMock>());
      contentManagerMock = new ContentManagerMock();
      storageMock = new StorageMock();
      subject = std::make_unique<StreamingContentService>(streamingOptions, taskThreadPoolMock,
                                                          curlDownloaderMock, contentManagerMock, storageMock);
    }

    virtual void TearDown() {
      streamingOptions = nullptr;
      taskThreadPoolMock = nullptr;
      subject = nullptr;
      delete contentManagerMock;
      delete storageMock;
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

    std::string mockPlaylist(std::string mockFile) {
      std::ifstream t(mockFile);
      std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
      return str;
    }

    std::unique_ptr<StreamingContentService> subject;
    std::shared_ptr<StreamingOptions> streamingOptions;
    std::shared_ptr<TaskThreadPoolMock> taskThreadPoolMock;
    std::shared_ptr<CurlDownloaderMock> curlDownloaderMock;
    ContentManagerMock* contentManagerMock;
    StorageMock* storageMock;
};

TEST_F(StreamingContentServiceTest, CreateStreamingContentService) {
  ASSERT_NE(subject, nullptr);
}

TEST_F(StreamingContentServiceTest, RetrievesTheConfiguredPlaylistsAndQueuesTasksToProcess) {
  EXPECT_CALL(*taskThreadPoolMock, enqueue(_)).Times(2).WillRepeatedly(Return());

  subject->processConfiguredPlaylists();
}

TEST_F(StreamingContentServiceTest, UsingRemotePlaylistDownloadPlaylistIntoMemory) {
  std::string playlist = "http://localhost/my/playlist";

  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  std::string expectedContent = playlistContent;

  EXPECT_CALL(*curlDownloaderMock, download(_)).WillOnce(Return(playlistContent));

  std::shared_ptr<InMemoryPlaylist> result = subject->downloadPlaylist(playlist);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getContent().c_str(), expectedContent.c_str());
}

TEST_F(StreamingContentServiceTest, UsingInMemoryPlaylistContentCreatesCdsObjects) {
    std::string playlistContent = mockPlaylist("fixtures/remote-playlist.pls");
  std::shared_ptr<InMemoryPlaylist> inMemoryPlaylist = std::make_shared<InMemoryPlaylist>(std::move(playlistContent));

  std::shared_ptr<PlaylistParseResult> result = subject->parsePlaylist(std::move(inMemoryPlaylist));

  EXPECT_NE(result, nullptr);
  EXPECT_NE(result->getParentContainer(), nullptr);
  EXPECT_EQ(result->getParentContainer()->getTitle(), "Radio Playlist");
  EXPECT_EQ(result->getParentContainer()->getParentID(), 0);

  // Verify children objects
  EXPECT_EQ(result->getChildObjects()->size(), 12);
  for (const auto &obj : (*result->getChildObjects())) {
    EXPECT_STREQ(obj->getClass().c_str(), "object.item.audioItem"); // TODO: Playlist Item?
    EXPECT_GT(obj->getResourceCount(), 0);
    zmm::Ref<CdsResource> resource = obj->getResource(0);
    EXPECT_EQ(resource->getAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO)), "http-get:*:application/octet-stream:*");
    ASSERT_TRUE(obj->getAuxData(ONLINE_SERVICE_LAST_UPDATE).length() > 0);
    ASSERT_TRUE(obj->isVirtual());
  }
}

TEST_F(StreamingContentServiceTest, GivenListofContentObjectsPersistsThemToDatabase) {
  // Mock up the parent container with parent ID
  std::shared_ptr<CdsContainer> parentCds = std::make_shared<CdsContainer>();
  parentCds->setTitle("Radio Playlist");
  parentCds->setParentID(0);
  std::shared_ptr<PlaylistParseResult> parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  zmm::Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  parseResult->addItem(newObject);

  // Mock the returned container parent from DB
  int expParentId = 5;
  zmm::Ref<CdsObject> parent(new CdsContainer());
  parent->setParentID(expParentId);

  EXPECT_CALL(*contentManagerMock, addContainer(Eq(0), Eq("Radio Playlist"), Eq("object.container"))).WillOnce(Return(expParentId));
  EXPECT_CALL(*storageMock, loadObject(Eq(expParentId))).WillOnce(Return(parent));
  EXPECT_CALL(*contentManagerMock, addObject(_)).Times(1).WillRepeatedly(Return());

  unsigned long result = subject->persistPlaylist(parseResult);

  EXPECT_EQ(result, 2);
}


