#include <icons/icon_config.h>
#include <mxml/mxml.h>
#include <common.h>
#include <regex>
#include <string>
#include <fstream>
#include <streambuf>
#include "gtest/gtest.h"

using namespace ::testing;
using namespace zmm;
using namespace mxml;

class IconConfigTest : public ::testing::Test {

public:
    IconConfigTest() = default;

    virtual ~IconConfigTest() = default;

    virtual void SetUp() {}

    virtual void TearDown() {
      subject = nullptr;
    };

    Ref<Element> mockConfig(const std::string &fixtureFile) {
      // Load the fixture XML, and replace ICONS_DIR for testing
      std::ifstream t(fixtureFile);
      std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
      t.close();
      replaceAll(fileStr, "{{ICONS_DIR}}", std::string(ICONS_DIR));

      Ref<mxml::Document> rootDoc;
      Ref<mxml::Element> root;
      Ref<Parser> parser(new Parser());
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

    Ref<Element> config;
    std::unique_ptr<IconConfig> subject;
};

TEST_F(IconConfigTest, IdentifiesIconLoadingType) {
  Ref<Element> config = mockConfig("fixtures/static-list.xml");
  subject = std::make_unique<IconConfig>(config);

  icon_loading_type result = subject->getType();

  EXPECT_EQ(result, static_list);
}

TEST_F(IconConfigTest, LoadsStaticListOfIcons) {
  Ref<Element> config = mockConfig("fixtures/static-list.xml");
  subject = std::make_unique<IconConfig>(config);

  std::vector<std::shared_ptr<GerberaIcon>> result = subject->getIcons();
  const auto &icon = subject->getIcons().at(0);
  std::string expPath = std::string(ICONS_DIR) + "/mt-icon120.png";
  EXPECT_STREQ(icon->path().c_str(), expPath.c_str());
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/png");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/mt-icon120.png");
  EXPECT_EQ(result.size(), 9);
}

#ifdef HAVE_MAGIC
TEST_F(IconConfigTest, LoadsDynamicListOfIconsLookingUpMimeTypesForPngJpgBmp) {
  Ref<Element> config = mockConfig("fixtures/dynamic-mimetypes-list.xml");
  subject = std::make_unique<IconConfig>(config);

  auto result = subject->getIcons();

  // PNG
  auto &icon = subject->getIcons().at(0);
  std::string expPath = std::string(ICONS_DIR) + "/mt-icon120.png";
  EXPECT_STREQ(icon->path().c_str(), expPath.c_str());
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/png");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/mt-icon120.png");
  // BMP
  icon = subject->getIcons().at(1);
  EXPECT_STREQ(icon->mimeType().c_str(), "image/x-ms-bmp");
  // JPG
  icon = subject->getIcons().at(2);
  EXPECT_STREQ(icon->mimeType().c_str(), "image/jpeg");
}
#endif

#if defined(HAVE_IMAGEMAGICK)
TEST_F(IconConfigTest, LoadsDynamicListOfIconsLookingUpResolutionUsingImageMagick) {
  Ref<Element> config = mockConfig("fixtures/dynamic-resolution-list.xml");
  subject = std::make_unique<IconConfig>(config);

  auto result = subject->getIcons();

  // PNG
  auto &icon = subject->getIcons().at(0);
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
}

TEST_F(IconConfigTest, GenerateImageConfigFromTemplateIcon) {
  Ref<Element> config = mockConfig("fixtures/dynamic-template.xml");
  subject = std::make_unique<IconConfig>(config);

  auto result = subject->getIcons();

  EXPECT_EQ(result.size(), 9);
  auto &icon = subject->getIcons().at(0);
  EXPECT_STREQ(icon->path().c_str(), "fixtures/icon-with-exif.jpg");
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/jpeg");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/grb-icon-120.jpg");
}
#endif