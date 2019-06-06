#include "gtest/gtest.h"
#include <icons/gerbera_icon.h>
#include <icons/static_icon.h>
#ifdef HAVE_IMAGEMAGICK
#include <Magick++/Image.h>
#endif


class StaticIconTest : public ::testing::Test {
public:
    StaticIconTest() = default;
    ~StaticIconTest() = default;
    void SetUp(){
      std::string path = "fixtures/icon-with-exif.jpg";
      std::string resolution = "120x120";
      std::string depth = "8";
      std::string mimeType = "image/png";
      std::string url = "content/icons/grb-icon-120.png";
      icon = std::make_shared<GerberaIcon>(path, resolution, depth, mimeType, url);
      subject = std::make_unique<StaticIcon>(icon);
    }
    void TearDown(){
      icon = nullptr;
      subject = nullptr;
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
    std::shared_ptr<GerberaIcon> icon;
    std::unique_ptr<StaticIcon> subject;
};

TEST_F(StaticIconTest, FileLength) {
  EXPECT_EQ(subject->fileLength(), 14583);
}
TEST_F(StaticIconTest, LastModified) {
  EXPECT_GT(subject->lastModified(), 0);
}
TEST_F(StaticIconTest, IsDirectory) {
  EXPECT_EQ(subject->isDirectory(), 0);
}
TEST_F(StaticIconTest, IsReadable) {
  EXPECT_EQ(subject->isReadable(), 1);
}
TEST_F(StaticIconTest, ContentType) {
  EXPECT_STREQ(subject->contentType(), "image/png");
}

#ifdef HAVE_IMAGEMAGICK
TEST_F(StaticIconTest, HandlerReadsImage) {
  size_t imgLength = 27836;
  zmm::Ref<IOHandler> result = subject->handler();
  result->open(UPNP_READ);
  Magick::Image image = readImageFromHandler(result, imgLength);

  EXPECT_EQ(image.baseColumns(), 120);
  EXPECT_EQ(image.baseRows(), 120);
  EXPECT_EQ(image.depth(), 8);
  EXPECT_STREQ("Joint Photographic Experts Group JFIF format", image.format().c_str());

  result->close();
}
#endif