#include <icons/icon_request_handler.h>
#include <icons/icon_config.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock/upnp_file_info_mock.h"
#include "mock/upnp_file_info_fixture.h"
#include <upnp/upnp.h>
#include <FileInfo.h>
#include <ixml.h>
#include <icons/icon_exception.h>

using namespace ::testing;
using namespace zmm;

class IconHandlerTest : public UpnpFileInfoFixture {
public:
    virtual void SetUp() {
      std::stringstream path;
      path << ICONS_DIR << DIR_SEPARATOR << "mt-icon32.jpg";
      config = new IconConfig(mockConfig("static-list", "/content/icons/mt-icon32.jpg", path.str()));
      subject = new IconRequestHandler(config);
    }

    virtual void TearDown() {
      delete config;
      delete subject;
    };

    zmm::Ref<Element> mockConfig(const std::string &loadingType, const std::string &url, const std::string &path) {
      zmm::Ref<Element> iconConf(new Element(_("icon-config")));
      iconConf->setAttribute("type", loadingType);
      zmm::Ref<Element> icons(new Element(_("icons")));
      zmm::Ref<Element> icon(new Element(_("icon")));
      icon->setText(path);
      icon->setAttribute(_("dimension"), _("120x120"));
      icon->setAttribute(_("depth"), _("24"));
      icon->setAttribute(_("mime-type"), _("image/jpeg"));
      icon->setAttribute(_("url"), url);
      icons->appendElementChild(icon);

      iconConf->appendElementChild(icons);
      return iconConf;
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

  subject->getInfo(cstr, info);

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 1)).WillRepeatedly(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_LastModified(Eq(info), _)).WillRepeatedly(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsDirectory(Eq(info), _)).WillRepeatedly(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_FileLength(Eq(info), _)).WillRepeatedly(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_ContentType(Eq(info), Eq("image/jpeg"))).WillRepeatedly(Return(1));
  delete [] cstr;
}

TEST_F(IconHandlerTest, SetsNotReadableWhenNotFound) {
  std::string url = "/content/icons/no-such-file.jpg";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  subject->getInfo(cstr, info);

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 0)).WillRepeatedly(Return(1));
  delete [] cstr;
}

TEST_F(IconHandlerTest, SetsNotReadableWhenFoundButNotAccessible) {
  std::string url = "/content/icons/no-such-file.jpg";
  std::stringstream path;
  path << ICONS_DIR << DIR_SEPARATOR << "no-such-file.jpg";
  config = new IconConfig(mockConfig("static-list", url, path.str()));
  subject = new IconRequestHandler(config);
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  subject->getInfo(cstr, info);

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 0)).WillRepeatedly(Return(1));
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

// 120x120 d24 image/png
// 120x120 image/x-ms-bmp
// 120x120 image/jpeg
// 48x48 d24
// 32x32 d8