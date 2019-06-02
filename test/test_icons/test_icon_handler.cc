#include <icons/icon_request_handler.h>
#include <icons/icon_config.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock/upnp_file_info_mock.h"
#include "mock/upnp_file_info_fixture.h"
#include <file_io_handler.h>
#include <upnp/upnp.h>
#include <FileInfo.h>
#include <ixml.h>
#include <fstream>
#include <streambuf>
#include <icons/icon_exception.h>
#ifdef HAVE_IMAGEMAGICK
#include <Magick++/Image.h>
#endif

using namespace ::testing;
using namespace zmm;

class IconHandlerTest : public UpnpFileInfoFixture {
public:
    virtual void SetUp() {
      std::stringstream path;
      path << ICONS_DIR << DIR_SEPARATOR << "mt-icon32.jpg";
      config = new IconConfig(mockConfig("fixtures/static-list.xml"));
      imgHelper = std::make_shared<ImageHelper>();
      subject = new IconRequestHandler(config, imgHelper);
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

    size_t getFileSize(const std::string &filename) {
      struct stat stat_buff;
      stat(filename.c_str(), &stat_buff);
      return stat_buff.st_size;
    }

#ifdef HAVE_IMAGEMAGICK
    Magick::Image readImageFromHandler(zmm::Ref<IOHandler> handler, size_t size) {
      // Read the image into a buffer
      char *buffer = new char[size];
      handler->read(buffer, size);
      // Make Buffer an Image
      Magick::Blob blob(static_cast<const void *>(buffer), size);
      Magick::Image image;
      image.read(blob);
      delete [] buffer;
      return image;
    }
#endif

    IconConfig *config;
    IconRequestHandler *subject;
    std::shared_ptr<ImageHelper> imgHelper;
};

TEST_F(IconHandlerTest, CreatesHandlerInstance) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(IconHandlerTest, SetsInfoForIconFromFileWhenAccessible) {
  std::stringstream ss;
  ss << ICONS_DIR << DIR_SEPARATOR << "mt-icon32.jpg";
  size_t fileSize = getFileSize(ss.str());
  std::string url = "/content/icons/mt-icon32.jpg";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 1)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_LastModified(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsDirectory(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_FileLength(Eq(info), Eq(fileSize))).WillOnce(Return(1));
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
  subject = new IconRequestHandler(config, imgHelper);
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 0)).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}

TEST_F(IconHandlerTest, SetsMimeTypeAndReadable) {
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

TEST_F(IconHandlerTest, OpenThrowsExceptionWhenIconNotFound) {
  std::string url = "/content/icons/no-such-file.jpg";
  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());

  EXPECT_THROW(subject->open(cstr, mode, range), IconException);
  delete [] cstr;
}

#ifdef HAVE_IMAGEMAGICK
TEST_F(IconHandlerTest, SetsInfoForDynamicIconWhenAccessible) {
  std::stringstream ss;
  Magick::Blob blob;
  imgHelper->convertTo("fixtures/icon-with-exif.jpg", blob, "image/png");
  Magick::Blob resizedBlob;
  imgHelper->resizeTo(blob, resizedBlob, 120, 120);
  size_t dynamicFileSize = resizedBlob.length();
  std::string url = "/content/icons/grb-icon-120.png";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  UpnpFileInfo *info = nullptr;
  config = new IconConfig(mockConfig("fixtures/dynamic-template.xml"));
  subject = new IconRequestHandler(config, imgHelper);

  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsReadable(Eq(info), 1)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_LastModified(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_IsDirectory(Eq(info), _)).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_FileLength(Eq(info), Eq(dynamicFileSize))).WillOnce(Return(1));
  EXPECT_CALL(*_upnpFileInfoMock, UpnpFileInfo_set_ContentType(Eq(info), StrEq("image/png"))).WillOnce(Return(1));

  subject->getInfo(cstr, info);

  delete [] cstr;
}

TEST_F(IconHandlerTest, WhenStaticListLoadsFileHandlerUsingFilePath) {
  std::string url = "/content/icons/mt-icon120.png";
  std::stringstream expectedIconFile;
  expectedIconFile << ICONS_DIR << DIR_SEPARATOR << "mt-icon120.png";
  size_t imgLength = getFileSize(expectedIconFile.str());

  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());

  // Open the image
  zmm::Ref<IOHandler> result = subject->open(cstr, mode, range);
  Magick::Image image = readImageFromHandler(result, imgLength);

  EXPECT_EQ(image.baseColumns(), 120);
  EXPECT_EQ(image.baseRows(), 120);
  EXPECT_EQ(image.depth(), 8);
  EXPECT_STREQ("Portable Network Graphics", image.format().c_str());

  result->close();
  delete [] cstr;
}

TEST_F(IconHandlerTest, ConvertsJpgToPngWhenDynamicList) {
  std::string url = "/content/icons/grb-icon-120.png";
  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  size_t imgLength = 27836;
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  config = new IconConfig(mockConfig("fixtures/dynamic-template.xml"));
  subject = new IconRequestHandler(config, imgHelper);

  zmm::Ref<IOHandler> result = subject->open(cstr, mode, range);
  Magick::Image image = readImageFromHandler(result, imgLength);

  EXPECT_EQ(image.baseColumns(), 120);
  EXPECT_EQ(image.baseRows(), 120);
  EXPECT_EQ(image.depth(), 8);
  EXPECT_STREQ("Portable Network Graphics", image.format().c_str());

  result->close();
  delete [] cstr;
}

TEST_F(IconHandlerTest, ConvertsJpgToBmpWhenDynamicList) {
  std::string url = "/content/icons/grb-icon-120.bmp";
  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  size_t imgLength = 43338;
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  config = new IconConfig(mockConfig("fixtures/dynamic-template.xml"));
  subject = new IconRequestHandler(config, imgHelper);

  zmm::Ref<IOHandler> result = subject->open(cstr, mode, range);
  Magick::Image image = readImageFromHandler(result, imgLength);

  EXPECT_EQ(image.baseColumns(), 120);
  EXPECT_EQ(image.baseRows(), 120);
  EXPECT_EQ(image.depth(), 8);
  EXPECT_STREQ("Microsoft Windows bitmap image", image.format().c_str());

  result->close();
  delete [] cstr;
}

TEST_F(IconHandlerTest, ResizesIconto48x48WhenDynamicList) {
  std::string url = "/content/icons/grb-icon-48.png";
  UpnpOpenFileMode mode = UPNP_READ;
  zmm::String range = "";
  size_t imgLength = 27836;
  char *cstr = new char[url.length() + 1];
  strcpy(cstr, url.c_str());
  config = new IconConfig(mockConfig("fixtures/dynamic-template.xml"));
  subject = new IconRequestHandler(config, imgHelper);

  zmm::Ref<IOHandler> result = subject->open(cstr, mode, range);
  Magick::Image image = readImageFromHandler(result, imgLength);

  EXPECT_EQ(image.baseColumns(), 48);
  EXPECT_EQ(image.baseRows(), 48);
  EXPECT_EQ(image.depth(), 8);
  EXPECT_STREQ("Portable Network Graphics", image.format().c_str());

  result->close();
  delete [] cstr;
}
#endif