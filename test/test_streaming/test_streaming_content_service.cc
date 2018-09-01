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
#include <timer.h>

#include "mock/task_threadpool_mock.h"
#include "mock/content_manager_mock.h"
#include "mock/storage_mock.h"
#include "mock/curl_downloader_mock.h"

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

    zmm::Ref<Element> mockConfig(std::string enabledShoutcast, std::string updateAtStart = "yes") {
      zmm::Ref<Element> streaming(new Element(_("streaming")));

      zmm::Ref<Element> playlists(new Element(_("playlists")));
      playlists->setAttribute("root-virtual-path", "/Root Virtual Path");
      playlists->setAttribute(_("update-at-start"), _(updateAtStart.c_str()));
      streaming->appendElementChild(playlists);

      zmm::Ref<Element> playlist(new Element(_("playlist")));
      playlist->setAttribute(_("url"), _("http://localhost/playlist"));
      playlist->setAttribute(_("name"), _("Name of Playlist"));
      playlist->setAttribute(_("purge-after"), _("43200"));
      playlists->appendElementChild(playlist);

      zmm::Ref<Element> playlist2(new Element(_("playlist")));
      playlist2->setAttribute(_("url"), _("http://localhost/playlist2"));
      playlist2->setAttribute(_("name"), _("Name of Playlist2"));
      playlist2->setAttribute(_("purge-after"), _("43200"));
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

TEST_F(StreamingContentServiceTest, ProcessesPlaylistsOnStartupWhenEnabled) {
  EXPECT_CALL(*taskThreadPoolMock, enqueue(_)).Times(2).WillRepeatedly(Return());

  subject->startupPlaylists();
}

TEST_F(StreamingContentServiceTest, DoesNotProcessPlaylistsOnStartupWhenDisabled) {
  streamingOptions = std::make_shared<StreamingOptions>(mockConfig("yes", "no"));
  EXPECT_CALL(*taskThreadPoolMock, enqueue(_)).Times(0);
  subject = std::make_unique<StreamingContentService>(streamingOptions, taskThreadPoolMock,
                                                      curlDownloaderMock, contentManagerMock, storageMock);

  subject->startupPlaylists();
}

TEST_F(StreamingContentServiceTest, ProcessesPlaylistsWhenTimerNotifyTriggers) {
  EXPECT_CALL(*taskThreadPoolMock, enqueue(_)).Times(2).WillRepeatedly(Return());
  zmm::Ref<Timer::Parameter> pl_param(new Timer::Parameter(Timer::Parameter::IDOnlineContent, OS_Playlists));

  subject->timerNotify(pl_param);
}

TEST_F(StreamingContentServiceTest, UsingRemotePlaylistDownloadPlaylistIntoMemory) {
  std::string playlist = "http://localhost/my/playlist";
  std::string name = "Playlist Name";

  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  std::string expectedContent = playlistContent;

  EXPECT_CALL(*curlDownloaderMock, download(_)).WillOnce(Return(playlistContent));

  std::shared_ptr<InMemoryPlaylist> result = subject->downloadPlaylist(name, playlist);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getContent().c_str(), expectedContent.c_str());
}

TEST_F(StreamingContentServiceTest, UsingInMemoryPlaylistCreateFromPLS) {
  std::string playlistContent = mockPlaylist("fixtures/example.pls");
  std::shared_ptr<InMemoryPlaylist> inMemoryPlaylist = std::make_shared<InMemoryPlaylist>("Name of Playlist", std::move(playlistContent));

  std::shared_ptr<PlaylistParseResult> result = subject->parsePlaylist(inMemoryPlaylist);

  EXPECT_NE(result, nullptr);
  EXPECT_NE(result->getParentContainer(), nullptr);
  EXPECT_EQ(result->getParentContainer()->getTitle(), "Name of Playlist");
  EXPECT_EQ(result->getParentContainer()->getParentID(), -1);

  // Verify children objects
  EXPECT_EQ(result->getChildObjects()->size(), 5);
  for (const auto &obj : (*result->getChildObjects())) {
    EXPECT_STREQ(obj->getClass().c_str(), UPNP_DEFAULT_CLASS_MUSIC_TRACK); // TODO: Playlist Item?
    EXPECT_GT(obj->getResourceCount(), 0);
    zmm::Ref<CdsResource> resource = obj->getResource(0);
    EXPECT_EQ(resource->getAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO)), "http-get:*:application/octet-stream:*");
    ASSERT_TRUE(obj->getAuxData(ONLINE_SERVICE_LAST_UPDATE).length() > 0);
    ASSERT_TRUE(obj->isVirtual());
  }
}

TEST_F(StreamingContentServiceTest, UsingInMemoryPlaylistCreateFromM3U) {
  std::string playlistContent = mockPlaylist("fixtures/example.m3u");
  std::shared_ptr<InMemoryPlaylist> inMemoryPlaylist = std::make_shared<InMemoryPlaylist>("Name of Playlist", std::move(playlistContent));

  std::shared_ptr<PlaylistParseResult> result = subject->parsePlaylist(inMemoryPlaylist);

  EXPECT_NE(result, nullptr);
  EXPECT_NE(result->getParentContainer(), nullptr);
  EXPECT_EQ(result->getParentContainer()->getTitle(), "Name of Playlist");
  EXPECT_EQ(result->getParentContainer()->getParentID(), -1);

  // Verify children objects
  EXPECT_EQ(result->getChildObjects()->size(), 8);
  for (const auto &obj : (*result->getChildObjects())) {
    EXPECT_STREQ(obj->getClass().c_str(), UPNP_DEFAULT_CLASS_MUSIC_TRACK);
    EXPECT_GT(obj->getResourceCount(), 0);
    zmm::Ref<CdsResource> resource = obj->getResource(0);
    EXPECT_EQ(resource->getAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO)), "http-get:*:application/octet-stream:*");
    ASSERT_TRUE(obj->getAuxData(ONLINE_SERVICE_LAST_UPDATE).length() > 0);
    ASSERT_TRUE(obj->isVirtual());
  }
}

TEST_F(StreamingContentServiceTest, UsingInMemoryPlaylistCreateFromXSPF) {
  std::string playlistContent = mockPlaylist("fixtures/example.xspf");
  std::shared_ptr<InMemoryPlaylist> inMemoryPlaylist = std::make_shared<InMemoryPlaylist>("Name of Playlist", std::move(playlistContent));

  std::shared_ptr<PlaylistParseResult> result = subject->parsePlaylist(inMemoryPlaylist);

  EXPECT_NE(result, nullptr);
  EXPECT_NE(result->getParentContainer(), nullptr);
  EXPECT_EQ(result->getParentContainer()->getTitle(), "Name of Playlist");
  EXPECT_EQ(result->getParentContainer()->getParentID(), -1);

  // Verify children objects
  EXPECT_EQ(result->getChildObjects()->size(), 2);
  for (const auto &obj : (*result->getChildObjects())) {
    EXPECT_STREQ(obj->getClass().c_str(), UPNP_DEFAULT_CLASS_MUSIC_TRACK); // TODO: Playlist Item?
    EXPECT_GT(obj->getResourceCount(), 0);
    zmm::Ref<CdsResource> resource = obj->getResource(0);
    EXPECT_EQ(resource->getAttribute(MetadataHandler::getResAttrName(R_PROTOCOLINFO)), "http-get:*:application/octet-stream:*");
    ASSERT_TRUE(obj->getAuxData(ONLINE_SERVICE_LAST_UPDATE).length() > 0);
    ASSERT_TRUE(obj->isVirtual());
  }
}

TEST_F(StreamingContentServiceTest, GivenListofContentObjectsPersistsUsingExistingRootButNoPlaylistContainer) {
  // Mock up the parent container with parent ID
  std::shared_ptr<CdsContainer> parentCds = std::make_shared<CdsContainer>();
  parentCds->setTitle("Radio Playlist");
  parentCds->setParentID(0);
  std::shared_ptr<PlaylistParseResult> parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  zmm::Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  parseResult->addItem(newObject);

  zmm::Ref<CdsObject> mockRootObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockRootObject->setID(555);

  zmm::Ref<CdsObject> mockPlaylistObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockPlaylistObject->setID(777);

  // Mock call to find existing parent - which will return result
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(Eq("/Root Virtual Path"))).WillOnce(Return(mockRootObject));

  // Mock call that will NOT find the playlist container
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(Eq("/Root Virtual Path/Radio Playlist"))).WillOnce(Return(nullptr));
  // Mock Playlist Container creation
  EXPECT_CALL(*contentManagerMock, addContainer(Eq(555), Eq("Radio Playlist"), Eq("object.container"))).WillOnce(Return(777));
  EXPECT_CALL(*storageMock, loadObject(_)).WillOnce(Return(mockPlaylistObject));
  EXPECT_CALL(*storageMock, updateObject(_,_)).WillOnce(Return());

  // Mock object stream creation
  EXPECT_CALL(*contentManagerMock, addObject(_)).Times(1).WillRepeatedly(Return());


  unsigned long result = subject->persistPlaylist(parseResult, 10);

  EXPECT_EQ(result, 1);
}

TEST_F(StreamingContentServiceTest, GivenListofContentObjectsPersistsUsingNewlyCreatedRootAndPlaylistContainer) {
  // Mock up the playlist container with parent ID
  std::shared_ptr<CdsContainer> parentCds = std::make_shared<CdsContainer>();
  parentCds->setTitle("Radio Playlist");
  parentCds->setParentID(0);

  std::shared_ptr<PlaylistParseResult> parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  zmm::Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  parseResult->addItem(newObject);

  // Mock call to find existing parent - which will return nullptr
  zmm::Ref<CdsObject> mockObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockObject->setID(555);
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(_)).Times(2).WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*storageMock, loadObject(_)).WillOnce(Return(mockObject));
  EXPECT_CALL(*storageMock, updateObject(_,_)).WillOnce(Return());

  // Mock root container creation
  int expRootContainerId = 555;
  EXPECT_CALL(*contentManagerMock, addContainerChain(Eq("/Root Virtual Path"), Eq(nullptr),
          Eq(INVALID_OBJECT_ID), Eq(nullptr))).WillOnce(Return(expRootContainerId));

  // Mock Playlist Container creation
  int expPlaylistContainerId = 777;
  EXPECT_CALL(*contentManagerMock, addContainer(Eq(expRootContainerId), Eq("Radio Playlist"),
          Eq("object.container"))).WillOnce(Return(expPlaylistContainerId));

  // Mock object creation
  EXPECT_CALL(*contentManagerMock, addObject(_)).Times(1).WillRepeatedly(Return());

  unsigned long objectsAdded = subject->persistPlaylist(parseResult, 10);

  EXPECT_EQ(objectsAdded, 1);
}

TEST_F(StreamingContentServiceTest, RemovesExistingPlaylistWhenExpired) {
  // Mock up the parent container with parent ID
  std::shared_ptr<CdsContainer> parentCds = std::make_shared<CdsContainer>();
  parentCds->setTitle("Radio Playlist");
  parentCds->setParentID(0);
  std::shared_ptr<PlaylistParseResult> parseResult = std::make_shared<PlaylistParseResult>(parentCds);
  zmm::Ref<CdsItemExternalURL> newObject = zmm::Ref<CdsItemExternalURL>(new CdsItemExternalURL());
  parseResult->addItem(newObject);

  zmm::Ref<CdsObject> mockRootObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockRootObject->setID(555);

  zmm::Ref<CdsObject> mockPlaylistObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockPlaylistObject->setID(777);
  struct timespec current;
  getTimespecNow(&current);
  mockPlaylistObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(current.tv_sec - 40));

  // Mock call to find existing parent - which will return result
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(Eq("/Root Virtual Path"))).WillOnce(Return(mockRootObject));

  // Mock call that will NOT find the playlist container
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(Eq("/Root Virtual Path/Radio Playlist"))).WillOnce(Return(mockPlaylistObject));
  EXPECT_CALL(*contentManagerMock, removeObject(Eq(777), Eq(false), Eq(true))).WillOnce(Return());

  // Mock Playlist Container creation
  EXPECT_CALL(*contentManagerMock, addContainer(Eq(555), Eq("Radio Playlist"), Eq("object.container"))).WillOnce(Return(777));
  EXPECT_CALL(*storageMock, loadObject(_)).WillOnce(Return(mockPlaylistObject));
  EXPECT_CALL(*storageMock, updateObject(_,_)).WillOnce(Return());

  // Mock object stream creation
  EXPECT_CALL(*contentManagerMock, addObject(_)).Times(1).WillRepeatedly(Return());


  unsigned long result = subject->persistPlaylist(parseResult, 30);

  EXPECT_EQ(result, 1);
}

TEST_F(StreamingContentServiceTest, ShouldProcessPlaylistWhenNotFoundInDatabase) {
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(_)).WillOnce(Return(nullptr));

  bool result = subject->shouldProcessPlaylist("Name of Playlist", 300);

  EXPECT_TRUE(result);
}

TEST_F(StreamingContentServiceTest, ShouldNotProcessPlaylistWhenFoundAndNotExpired) {
  zmm::Ref<CdsObject> mockObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockObject->setID(555);
  struct timespec current;
  getTimespecNow(&current);
  mockObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(current.tv_sec));
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(_)).WillOnce(Return(mockObject));

  bool result = subject->shouldProcessPlaylist("Name of Playlist", 30);

  EXPECT_FALSE(result);
}

TEST_F(StreamingContentServiceTest, ShouldProcessPlaylistWhenFoundAndExpired) {
  zmm::Ref<CdsObject> mockObject = zmm::Ref<CdsObject>(new CdsContainer());
  mockObject->setID(555);
  struct timespec current;
  getTimespecNow(&current);
  mockObject->setAuxData(_(ONLINE_SERVICE_LAST_UPDATE), String::from(current.tv_sec - 300));
  EXPECT_CALL(*storageMock, findVirtualObjectByPath(_)).WillOnce(Return(mockObject));

  bool result = subject->shouldProcessPlaylist("Name of Playlist", 30);

  EXPECT_TRUE(result);
}


