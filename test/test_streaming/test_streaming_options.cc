#include <streaming/streaming_options.h>
#include <xpath.h>
#include <common.h>
#include <streaming/shoutcast_options.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using namespace zmm;
using namespace mxml;

class StreamingOptionsTest : public ::testing::Test {

 public:

  StreamingOptionsTest() {};

  virtual ~StreamingOptionsTest() {};

  virtual void SetUp() {
    Ref<Element> streaming = mockConfig("yes");
    subject = new StreamingOptions(streaming);
  }

  virtual void TearDown() {
    delete subject;
  }

  Ref<Element> mockConfig(std::string enabledShoutcast) {
    Ref<Element> streaming(new Element(_("streaming")));

    Ref<Element> playlists(new Element(_("playlists")));
    streaming->appendElementChild(playlists);

    Ref<Element> playlist(new Element(_("playlist")));
    playlist->setAttribute(_("url"), _("http://localhost/playlist"));
    playlists->appendElementChild(playlist);

    Ref<Element> shoutcast(new Element(_("shoutcast")));
    shoutcast->setAttribute(_("base-url"), _("http://api.shoutcast.com"));
    shoutcast->setAttribute(_("dev-id"), _("ABC123"));
    shoutcast->setAttribute(_("enabled"), _(enabledShoutcast.c_str()));

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

  StreamingOptions *subject;
};

TEST_F(StreamingOptionsTest, ConvertsXmlOptionsIntoObjectDefinition) {
  ASSERT_NE(subject, nullptr);
  EXPECT_EQ(1, subject->getPlaylists()->getSize());
  auto shoutcast = subject->getShoutcastOptions();
  ASSERT_TRUE(shoutcast->isEnabled());
  EXPECT_STREQ(shoutcast->getDevId().c_str(), "ABC123");
}

TEST_F(StreamingOptionsTest, WhenEnabledNoSetsBooleanToFalse) {
  Ref<Element> config = mockConfig("no");
  subject = new StreamingOptions(config);
  auto shoutcast = subject->getShoutcastOptions();
  ASSERT_FALSE(shoutcast->isEnabled());
}

