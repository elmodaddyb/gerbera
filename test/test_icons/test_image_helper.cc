#include <common.h>
#include <icons/image_helper.h>
#include "gtest/gtest.h"

using namespace ::testing;

class ImageHelperTest: public ::testing::Test {
public:
    ImageHelperTest() = default;
    ~ImageHelperTest() override = default ;
    void SetUp() override {
      testIcon = "fixtures/icon-with-exif.jpg";
    }
    void TearDown() override { };
    std::string testIcon;
};

#ifdef HAVE_IMAGEMAGICK
TEST_F(ImageHelperTest, RetrievesImageDetailsUsingImageMagick) {
  imageDetails details = ImageHelper::readFromMagick(testIcon);
  EXPECT_TRUE(details.valid);
}

TEST_F(ImageHelperTest, ImageDetailsContainsWidthHeightDepth) {
  imageDetails details = ImageHelper::readFromMagick(testIcon);
  EXPECT_TRUE(details.valid);
  EXPECT_EQ(details.width, 120);
  EXPECT_EQ(details.height, 120);
  EXPECT_EQ(details.depth, 8);
}
#endif

#ifdef HAVE_MAGIC
TEST_F(ImageHelperTest, RetievesMimeTypeUsingMagic) {
  std::string mime = ImageHelper::mimeFromMagic(testIcon);
  EXPECT_EQ(mime, "image/jpeg");
}
#endif

#ifdef HAVE_LIBEXIF
TEST_F(ImageHelperTest, RetrievesImageDetailsUsingExifTags) {
  imageDetails details = ImageHelper::readFromExif(testIcon);
  EXPECT_TRUE(details.valid);
  EXPECT_EQ(details.width, 120);
  EXPECT_EQ(details.height, 120);
  EXPECT_EQ(details.depth, 8);
}
#endif