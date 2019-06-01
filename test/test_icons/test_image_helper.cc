#include <common.h>
#include <icons/image_helper.h>
#include <Magick++/Blob.h>
#include <Magick++/Image.h>
#include "gtest/gtest.h"

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
  imageDetails details = subject->readFromMagick(testIcon);
  EXPECT_TRUE(details.valid);
}

TEST_F(ImageHelperTest, ImageDetailsContainsWidthHeightDepth) {
  imageDetails details = subject->readFromMagick(testIcon);
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
#endif

#ifdef HAVE_MAGIC
TEST_F(ImageHelperTest, RetievesMimeTypeUsingMagic) {
  std::string mime = subject->mimeFromMagic(testIcon);
  EXPECT_EQ(mime, "image/jpeg");
}
#endif

#ifdef HAVE_LIBEXIF
TEST_F(ImageHelperTest, RetrievesImageDetailsUsingExifTags) {
  imageDetails details = subject->readFromExif(testIcon);
  EXPECT_TRUE(details.valid);
  EXPECT_EQ(details.width, 120);
  EXPECT_EQ(details.height, 120);
  EXPECT_EQ(details.depth, 8);
}
#endif