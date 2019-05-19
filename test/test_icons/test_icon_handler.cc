#include <icons/icon_request_handler.h>
#include "gtest/gtest.h"

using namespace ::testing;

class IconHandlerTest : public ::testing::Test {

public:
    IconHandlerTest() {};

    virtual ~IconHandlerTest() {};

    virtual void SetUp() {
      subject = new IconRequestHandler();
    }

    virtual void TearDown() {
      delete subject;
    };

    IconRequestHandler *subject;
};

TEST_F(IconHandlerTest, CreatesHandlerInstance) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(IconHandlerTest, ReturnsIOHandlerForIcon) {
  std::string filename = "/content/icons/mt-icon32.png";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());
  UpnpOpenFileMode fileMode = UPNP_READ;
  zmm::String range = "";

  zmm::Ref<IOHandler> result = subject->open(cstr, fileMode, range);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}