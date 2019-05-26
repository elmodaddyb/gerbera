#include <icons/icon_request_handler.h>
#include <icons/icon_config.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock/upnp_file_info_mock.h"
#include "mock/upnp_file_info_fixture.h"
#include <upnp/upnp.h>
#include <FileInfo.h>
#include <ixml.h>
#include <fstream>
#include <streambuf>
#include <icons/icon_exception.h>

using namespace ::testing;
using namespace zmm;

class IconHandlerTest : public UpnpFileInfoFixture {
public:
    virtual void SetUp() {
      std::stringstream path;
      path << ICONS_DIR << DIR_SEPARATOR << "mt-icon32.jpg";
      config = new IconConfig(mockConfig("fixtures/static-list.xml"));
      subject = new IconRequestHandler(config);
    }

    virtual void TearDown() {
      delete config;
      delete subject;
    };

    zmm::Ref<Element> mockConfig(const std::string &fixtureFile) {
      // Load the fixture XML, and replace ICONS_DIR for testing
      std::ifstream t(fixtureFile);
      std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
      t.close();
      replaceAll(fileStr, "{{ICONS_DIR}}", std::string(ICONS_DIR));

      zmm::Ref<mxml::Document> rootDoc;
      zmm::Ref<mxml::Element> root;
      zmm::Ref<Parser> parser(new Parser());
      rootDoc = parser->parseString(fileStr);
      root = rootDoc->getRoot();
      return root;
    }

    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
      if(from.empty())
        return;
      size_t start_pos = 0;
      while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
      }
    }
    IconConfig *config;
    IconRequestHandler *subject;
};

TEST_F(IconHandlerTest, CreatesHandlerInstance) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(IconHandlerTest, SetsInfoForIconWhenAccessible) {
  std::string url = "/content/icons/mt-icon32.jpg";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 1)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_LastModified(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsDirectory(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_FileLength(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_ContentType(Eq(info), StrEq("image/jpeg"))).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}

TEST_F(IconHandlerTest, SetsNotReadableWhenNotFound) {
  std::string url = "/content/icons/no-such-file.jpg";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 0)).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}

TEST_F(IconHandlerTest, SetsNotReadableWhenFoundButNotAccessible) {
  std::string url = "/content/icons/no-such-file.jpg";
  config = new IconConfig(mockConfig("fixtures/no-such-file.xml"));
  subject = new IconRequestHandler(config);
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 0)).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}

TEST_F(IconHandlerTest, OpenThrowsExceptionWhenIconNotFound) {
  std::string url = "/content/icons/no-such-file.jpg";
  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());

  EXPECT_THROW(subject->open(cstr, mode, range), IconException);
  delete [] cstr;
}

#if defined(HAVE_MAGIC)
TEST_F(IconHandlerTest, RetrievesMimeTypeFromMagic) {
  std::string url = "/content/icons/mt-icon32.png";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 1)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_LastModified(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsDirectory(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_FileLength(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_ContentType(Eq(info), StrEq("image/png"))).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}
#endif

// 120x120 d24 image/png
// 120x120 image/x-ms-bmp
// 120x120 image/jpeg
// 48x48 d24
// 32x32 d8