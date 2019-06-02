#include "gtest/gtest.h"
#include <icons/icon_config.h>
#include <icons/icon_xml_helper.h>
#include <fstream>
#include <streambuf>

using namespace ::testing;
using namespace zmm;
using namespace mxml;

class IconXmlHelperTest : public ::testing::Test {
public:
    IconXmlHelperTest() = default;

    virtual ~IconXmlHelperTest() = default;

    virtual void SetUp() {
      subject = new IconXmlHelper();
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

    IconConfig* config;
    IconXmlHelper* subject;
};

TEST_F(IconXmlHelperTest, GeneratesDefaultIconXmlWhenNotConfigured) {
  config = nullptr;
  Ref<Element> iconList = subject->generateDescList(config);

  EXPECT_NE(iconList, nullptr);
  EXPECT_EQ(iconList->elementChildCount(), 9);
}
#ifdef HAVE_IMAGEMAGICK
TEST_F(IconXmlHelperTest, GeneratesIconXmlWithDynamicIconList) {
  Ref<Element> mockCfg = mockConfig("fixtures/dynamic-template.xml");
  config = new IconConfig(mockCfg);
  Ref<Element> iconList = subject->generateDescList(config);

  EXPECT_NE(iconList, nullptr);
  EXPECT_EQ(iconList->elementChildCount(), 9);

  // First Icon
  Ref<Element> icon = iconList->getElementChild(0);
  EXPECT_STREQ("120", icon->getChildByName("width")->getText().c_str());
  EXPECT_STREQ("120", icon->getChildByName("height")->getText().c_str());
  EXPECT_STREQ("24", icon->getChildByName("depth")->getText().c_str());
  EXPECT_STREQ("/content/icons/grb-icon-120.jpg", icon->getChildByName("url")->getText().c_str());
  EXPECT_STREQ("image/jpeg", icon->getChildByName("mimetype")->getText().c_str());

  // Second Icon
  icon = iconList->getElementChild(1);
  EXPECT_STREQ("120", icon->getChildByName("width")->getText().c_str());
  EXPECT_STREQ("120", icon->getChildByName("height")->getText().c_str());
  EXPECT_STREQ("24", icon->getChildByName("depth")->getText().c_str());
  EXPECT_STREQ("/content/icons/grb-icon-120.png", icon->getChildByName("url")->getText().c_str());
  EXPECT_STREQ("image/png", icon->getChildByName("mimetype")->getText().c_str());

  // Third Icon
  icon = iconList->getElementChild(2);
  EXPECT_STREQ("120", icon->getChildByName("width")->getText().c_str());
  EXPECT_STREQ("120", icon->getChildByName("height")->getText().c_str());
  EXPECT_STREQ("24", icon->getChildByName("depth")->getText().c_str());
  EXPECT_STREQ("/content/icons/grb-icon-120.bmp", icon->getChildByName("url")->getText().c_str());
  EXPECT_STREQ("image/x-ms-bmp", icon->getChildByName("mimetype")->getText().c_str());
}
#endif