#include <streaming/streaming_options.h>
#include <xpath.h>
#include <common.h>
#include <streaming/shoutcast_options.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

using namespace std;
using namespace zmm;
using namespace mxml;

class StreamingOptionsTest : public ::testing::Test {

 public:

  StreamingOptionsTest() {};

  virtual ~StreamingOptionsTest() {};

  virtual void SetUp() {
  }

  virtual void TearDown() {
    subject = nullptr;
  }

  Ref<Element> mockConfig(std::string enabledShoutcast) {
    Ref<Element> streaming(new Element(_("streaming")));

    Ref<Element> playlists(new Element(_("playlists")));
    streaming->appendElementChild(playlists);
    playlists->setAttribute(_("rootVirtualPath"), _("/Radio Playlists"));

    Ref<Element> playlist(new Element(_("playlist")));
    playlist->setAttribute(_("url"), _("http://localhost/playlist"));
    playlists->appendElementChild(playlist);

    Ref<Element> shoutcast(new Element(_("shoutcast")));
    shoutcast->setAttribute(_("base-url"), _("http://api.shoutcast.com"));
    shoutcast->setAttribute(_("dev-id"), _("ABC123"));
    shoutcast->setAttribute(_("enabled"), String::copy(enabledShoutcast.c_str()));

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
  EXPECT_EQ(1, subject->getPlaylists()->getSize());
  auto shoutcast = subject->getShoutcastOptions();
  ASSERT_TRUE(shoutcast->isEnabled());
  EXPECT_STREQ(shoutcast->getDevId().c_str(), "ABC123");
}

TEST_F(StreamingOptionsTest, WhenEnabledNoSetsBooleanToFalse) {
  Ref<Element> config = mockConfig("no");
  subject = make_unique<StreamingOptions>(config);
  auto shoutcast = subject->getShoutcastOptions();
  ASSERT_FALSE(shoutcast->isEnabled());
}

TEST_F(StreamingOptionsTest, ContainsRootVirtualPathForPlaylists) {
  Ref<Element> config = mockConfig("no");
  subject = make_unique<StreamingOptions>(config);
  auto playlists = subject->getPlaylists();
  ASSERT_STREQ(playlists->getRootVirtualPath().c_str(), "/Radio Playlists");
}

