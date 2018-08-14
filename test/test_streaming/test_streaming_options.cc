#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>
#include <xpath.h>
#include <common.h>
#include <streaming/streaming_options.h>

using namespace std;
using namespace zmm;
using namespace mxml;

class StreamingOptionsTest : public ::testing::Test {

 public:

  StreamingOptionsTest() = default;

  virtual ~StreamingOptionsTest() = default;

  virtual void SetUp() override {
  }

  virtual void TearDown() override {
    subject = nullptr;
  }

  Ref<Element> mockConfig(std::string enabledShoutcast) {
    Ref<Element> streaming(new Element(_("streaming")));

    Ref<Element> playlists(new Element(_("playlists")));
    streaming->appendElementChild(playlists);
    playlists->setAttribute(_("root-virtual-path"), _("/Radio Playlists"));

    Ref<Element> playlist(new Element(_("playlist")));
    playlist->setAttribute(_("url"), _("http://localhost/playlist"));
    playlist->setAttribute(_("name"), _("Name of Playlist"));
    playlists->appendElementChild(playlist);

    Ref<Element> shoutcast(new Element(_("shoutcast")));
    shoutcast->setAttribute(_("base-url"), _("http://api.shoutcast.com"));
    shoutcast->setAttribute(_("dev-id"), _("ABC123"));
    shoutcast->setAttribute(_("enabled"), String::copy(std::move(enabledShoutcast).c_str()));

    // TODO: Enable shoutcase features.
//    Ref<Element> genre(new Element(_("genre")));
//    genre->setAttribute(_("genre"), _("classic"));
//    genre->setAttribute(_("limit"), _("50"));
//    genre->setAttribute(_("bit-rate"), _("64"));
//    genre->setAttribute(_("media-type"), _("audio/mpeg"));
//    genre->setAttribute(_("container-name"), _("My Classic Stations"));
//    shoutcast->appendElementChild(genre);

    streaming->appendElementChild(shoutcast);
    return streaming;
  }

  std::unique_ptr<StreamingOptions> subject;
};

TEST_F(StreamingOptionsTest, ConvertsXmlOptionsIntoObjectDefinition) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  ASSERT_NE(subject, nullptr);
  EXPECT_EQ(1, subject->playlists()->getSize());
  auto shoutcast = subject->shoutcastOptions();
  ASSERT_TRUE(shoutcast->isEnabled());
  EXPECT_STREQ(shoutcast->getDevId().c_str(), "ABC123");
}

TEST_F(StreamingOptionsTest, WhenEnabledNoSetsBooleanToFalse) {
  Ref<Element> config = mockConfig("no");
  subject = make_unique<StreamingOptions>(config);
  auto shoutcast = subject->shoutcastOptions();
  ASSERT_FALSE(shoutcast->isEnabled());
}

TEST_F(StreamingOptionsTest, ContainsRootVirtualPathForPlaylists) {
  Ref<Element> config = mockConfig("no");
  subject = make_unique<StreamingOptions>(config);
  auto playlists = subject->playlists();
  ASSERT_STREQ(playlists->getRootVirtualPath().c_str(), "/Radio Playlists");
}

TEST_F(StreamingOptionsTest, CreatesStreamingPlaylistWithPlaylists) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlistOptions = subject->playlists();
  ASSERT_EQ(1, playlistOptions->getSize());
}

TEST_F(StreamingOptionsTest, AllowsToAddConfiguredPlaylists) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlist = make_unique<StreamingOptions::ConfiguredPlaylist>("http://localhost", "Name of Playlist");
  auto playlistOptions = subject->playlists();

  playlistOptions->addPlaylist(std::move(playlist));

  ASSERT_EQ(2, playlistOptions->getSize());
}

TEST_F(StreamingOptionsTest, ProvidesListOfAllPlaylists) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlist = make_unique<StreamingOptions::ConfiguredPlaylist>("http://localhost", "Name of Playlist");
  auto playlistOptions = subject->playlists();

  playlistOptions->addPlaylist(std::move(playlist));

  auto playlists = playlistOptions->getPlaylists();

  ASSERT_EQ(2, playlists->size());
  EXPECT_STREQ("http://localhost/playlist", playlists->at(0)->getUrl().c_str());
}

TEST_F(StreamingOptionsTest, AllowsToAddSeveralConfiguredPlaylists) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlist = make_unique<StreamingOptions::ConfiguredPlaylist>("http://localhost/playlist1", "Name of Playlist");
  auto playlistOptions = subject->playlists();
  playlistOptions->addPlaylist(std::move(playlist));

  playlist = make_unique<StreamingOptions::ConfiguredPlaylist>("http://localhost/playlist2", "Name of Playlist");
  playlistOptions->addPlaylist(std::move(playlist));

  ASSERT_EQ(3, playlistOptions->getSize());
  ASSERT_STREQ("http://localhost/playlist", playlistOptions->getPlaylists()->at(0)->getUrl().c_str());
  ASSERT_STREQ("http://localhost/playlist1", playlistOptions->getPlaylists()->at(1)->getUrl().c_str());
  ASSERT_STREQ("http://localhost/playlist2", playlistOptions->getPlaylists()->at(2)->getUrl().c_str());
}

TEST_F(StreamingOptionsTest, ContainsVirtualPathForAllPlaylists) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlistOptions = subject->playlists();

  std::string result = playlistOptions->getRootVirtualPath();

  ASSERT_STREQ(result.c_str(), "/Radio Playlists");
}

TEST_F(StreamingOptionsTest, EachPlaylistHasName) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto playlist = make_unique<StreamingOptions::ConfiguredPlaylist>("http://localhost/playlist1", "Name of Playlist");
  auto playlistOptions = subject->playlists();
  playlistOptions->addPlaylist(std::move(playlist));

  std::string result = playlistOptions->getPlaylists()->at(0)->getName();

  ASSERT_STREQ(result.c_str(), "Name of Playlist");
}

TEST_F(StreamingOptionsTest, CreatesShoutcastOptionsWithDevIdUrlAndEnabled) {
  Ref<Element> config = mockConfig("yes");
  subject = make_unique<StreamingOptions>(config);
  auto shoutcastOptions = subject->shoutcastOptions();

  EXPECT_STREQ(shoutcastOptions->getBaseUrl().c_str(), "http://api.shoutcast.com");
  EXPECT_STREQ(shoutcastOptions->getDevId().c_str(), "ABC123");
  ASSERT_TRUE(shoutcastOptions->isEnabled());
}


