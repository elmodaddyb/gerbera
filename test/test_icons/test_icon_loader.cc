#include <icons/icon_loader.h>
#include <fstream>
#include <icons/gerbera_icon.h>
#include "gtest/gtest.h"

class IconLoaderTest: public ::testing::Test {
public:
    IconLoaderTest() = default;
    ~IconLoaderTest() = default;
    void SetUp() {
      subject = new IconLoader();
    }
    void TearDown() {
      delete subject;
    }

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

    IconLoader* subject;

};

TEST_F(IconLoaderTest, InvalidConfigIsUnsupportedLoadingType) {
  zmm::Ref<Element> config =  zmm::Ref<Element>(new Element(_("server")));

  IconLoader::Type result = subject->identifyLoadingType(config);

  EXPECT_EQ(result, IconLoader::Type::None);
}

TEST_F(IconLoaderTest, LoadingStaticTypeBasedOnServerConfigXml) {
  zmm::Ref<Element> config = mockConfig("fixtures/static-list.xml");

  IconLoader::Type result = subject->identifyLoadingType(config);

  EXPECT_EQ(result, IconLoader::Type::Static);
}

#ifdef HAVE_IMAGEMAGICK
TEST_F(IconLoaderTest, LoadingDynamicTypeBasedOnServerConfigXml) {
  zmm::Ref<Element> config = mockConfig("fixtures/dynamic-template.xml");

  IconLoader::Type result = subject->identifyLoadingType(config);

  EXPECT_EQ(result, IconLoader::Type::Dynamic);
}
#endif

TEST_F(IconLoaderTest, LoadsStaticListOfIcons) {
  zmm::Ref<Element> config = mockConfig("fixtures/static-list.xml");

  std::vector<std::shared_ptr<GerberaIcon>> result = subject->loadStaticList(config);
  const auto &icon = result.at(0);
  std::string expPath = std::string(ICONS_DIR) + "/mt-icon120.png";
  EXPECT_STREQ(icon->path().c_str(), expPath.c_str());
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/png");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/mt-icon120.png");
  EXPECT_EQ(result.size(), 9);
}

#ifdef HAVE_MAGIC
TEST_F(IconLoaderTest, LoadsStaticListOfIconsLookingUpMimeTypesForPngJpgBmp) {
  zmm::Ref<Element> config = mockConfig("fixtures/dynamic-mimetypes-list.xml");

  auto result =  subject->loadStaticList(config);

  // PNG
  auto &icon = result.at(0);
  std::string expPath = std::string(ICONS_DIR) + "/mt-icon120.png";
  EXPECT_STREQ(icon->path().c_str(), expPath.c_str());
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/png");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/mt-icon120.png");
  // BMP
  icon = result.at(1);
  EXPECT_STREQ(icon->mimeType().c_str(), "image/x-ms-bmp");
  // JPG
  icon = result.at(2);
  EXPECT_STREQ(icon->mimeType().c_str(), "image/jpeg");
}
#endif

#ifdef HAVE_IMAGEMAGICK
TEST_F(IconLoaderTest, LoadsStaticListOfIconsLookingUpResolutionUsingImageMagick) {
  zmm::Ref<Element> config = mockConfig("fixtures/dynamic-resolution-list.xml");

  auto result = subject->loadStaticList(config);

  // PNG
  auto &icon = result.at(0);
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
}

TEST_F(IconLoaderTest, GenerateImageConfigFromTemplateIcon) {
  zmm::Ref<Element> config = mockConfig("fixtures/dynamic-template.xml");

  auto result = subject->loadDynamicList(config);

  EXPECT_EQ(result.size(), 9);
  auto &icon = result.at(0);
  EXPECT_STREQ(icon->path().c_str(), "fixtures/icon-with-exif.jpg");
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/jpeg");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/grb-icon-120.jpg");
}

TEST_F(IconLoaderTest, GenerateImageConfigFromTemplateIconDelegateMethod) {
  zmm::Ref<Element> config = mockConfig("fixtures/dynamic-template.xml");

  auto result = subject->loadIconList(config);

  EXPECT_EQ(result.size(), 9);
  auto &icon = result.at(0);
  EXPECT_STREQ(icon->path().c_str(), "fixtures/icon-with-exif.jpg");
  EXPECT_STREQ(icon->resolution().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/jpeg");
  EXPECT_STREQ(icon->url().c_str(), "/content/icons/grb-icon-120.jpg");
}
#endif