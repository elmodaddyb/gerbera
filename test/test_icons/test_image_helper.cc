#include "gtest/gtest.h"
#include <common.h>
#include <icons/image_helper.h>
#ifdef HAVE_IMAGEMAGICK
#include <Magick++/Blob.h>
#include <Magick++/Image.h>
#endif

using namespace ::testing;

class ImageHelperTest: public ::testing::Test {
public:
    ImageHelperTest() = default;
    ~ImageHelperTest() override = default ;
    void SetUp() override {
      testIcon = "fixtures/icon-with-exif.jpg";
      subject = new ImageHelper();
    }
    void TearDown() override { };
    std::string testIcon;
    ImageHelper *subject;
};

#ifdef HAVE_IMAGEMAGICK
TEST_F(ImageHelperTest, RetrievesImageDetailsUsingImageMagick) {
  ImageDetails details = subject->readFromMagick(testIcon);
  EXPECT_TRUE(details.valid);
}

TEST_F(ImageHelperTest, ImageDetailsContainsWidthHeightDepth) {
  ImageDetails details = subject->readFromMagick(testIcon);
  EXPECT_TRUE(details.valid);
  EXPECT_EQ(details.width, 120);
  EXPECT_EQ(details.height, 120);
  EXPECT_EQ(details.depth, 8);
}

TEST_F(ImageHelperTest, ConvertsFileToPngType) {
  Magick::Blob blob;
  std::string orig = "fixtures/icon-with-exif.jpg";
  subject->convertTo(orig, blob, "image/png");
  Magick::Image image( blob );
  EXPECT_STREQ("Portable Network Graphics", image.format().c_str());
}

TEST_F(ImageHelperTest, ConvertsFileToBmpType) {
  Magick::Blob blob;
  std::string orig = "fixtures/icon-with-exif.jpg";
  subject->convertTo(orig, blob, "image/x-ms-bmp");
  Magick::Image image( blob );
  EXPECT_STREQ("Microsoft Windows bitmap image", image.format().c_str());
}

TEST_F(ImageHelperTest, ConvertsFileToJpgType) {
  Magick::Blob blob;
  std::string orig = "fixtures/icon-with-exif.jpg";
  subject->convertTo(orig, blob, "image/jpeg");
  Magick::Image image( blob );
  EXPECT_STREQ("Joint Photographic Experts Group JFIF format", image.format().c_str());
}

TEST_F(ImageHelperTest, ResizesToDimension) {
  Magick::Blob fromBlob;
  Magick::Blob toBlob;
  std::string orig = "fixtures/icon-with-exif.jpg";
  Magick::Image origImage(orig);
  origImage.write(&fromBlob);
  subject->resizeTo(fromBlob, toBlob, 48, 48);
  Magick::Image image( toBlob );
  EXPECT_EQ(image.baseColumns(), 48);
  EXPECT_EQ(image.baseRows(), 48);
}
#endif

#ifdef HAVE_MAGIC
TEST_F(ImageHelperTest, RetievesMimeTypeUsingMagic) {
  std::string mime = subject->mimeFromMagic(testIcon);
  EXPECT_EQ(mime, "image/jpeg");
}
#endif

#ifdef HAVE_LIBEXIF
TEST_F(ImageHelperTest, RetrievesImageDetailsUsingExifTags) {
  ImageDetails details = subject->readFromExif(testIcon);
  EXPECT_TRUE(details.valid);
  EXPECT_EQ(details.width, 120);
  EXPECT_EQ(details.height, 120);
  EXPECT_EQ(details.depth, 8);
}
#endif