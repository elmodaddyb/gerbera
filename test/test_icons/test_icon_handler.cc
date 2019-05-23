#include <icons/icon_request_handler.h>
#include <icons/icon_config.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock/upnp_file_info_mock.h"
#include "mock/upnp_file_info_fixture.h"
#include <upnp/upnp.h>
#include <FileInfo.h>
#include <ixml.h>

using namespace ::testing;


class IconHandlerTest : public UpnpFileInfoFixture {
public:
    virtual void SetUp() {
      config = new IconConfig();
      subject = new IconRequestHandler(config);
    }

    virtual void TearDown() {
      delete config;
      delete subject;
    };

    IconConfig *config;
    IconRequestHandler *subject;
};

TEST_F(IconHandlerTest, CreatesHandlerInstance) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(IconHandlerTest, GetsInfoForIcon) {
  std::string filename = "/content/icons/mt-icon32.png";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());
  UpnpFileInfo *info = nullptr;

  subject->getInfo(cstr, info);

  // TODO EXPECT assertions of MOCK
}

//TEST_F(IconHandlerTest, ReturnsIOHandlerForIcon) {
//  std::string filename = "/content/icons/mt-icon32.png";
//  char *cstr = new char[filename.length() + 1];
//  strcpy(cstr, filename.c_str());
//  UpnpOpenFileMode fileMode = UPNP_READ;
//  zmm::String range = "";
//
//  zmm::Ref<IOHandler> result = subject->open(cstr, fileMode, range);
//
//  EXPECT_NE(result, nullptr);
//  delete [] cstr;
//}

// 120x120 d24 image/png
// 120x120 image/x-ms-bmp
// 120x120 image/jpeg
// 48x48 d24
// 32x32 d8