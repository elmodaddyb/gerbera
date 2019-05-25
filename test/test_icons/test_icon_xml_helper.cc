#include "gtest/gtest.h"
#include <icons/icon_config.h>
#include <icons/icon_xml_helper.h>

using namespace ::testing;
using namespace zmm;
using namespace mxml;

class IconXmlHelperTest : public ::testing::Test {
public:
    IconXmlHelperTest() = default;

    virtual ~IconXmlHelperTest() = default;

    virtual void SetUp() {
      Ref<Element> mockCfg = mockConfig("static-list");
      config = new IconConfig(mockCfg);
      subject = new IconXmlHelper();
    }

    virtual void TearDown() {
      delete config;
      delete subject;
    };

    Ref<Element> mockConfig(std::string loadingType) {
      Ref<Element> iconConf(new Element(_("icon-config")));
      iconConf->setAttribute("type", loadingType);
      Ref<Element> icons(new Element(_("icons")));

      Ref<Element> icon(new Element(_("icon")));
      icon->setText(_("/icon/path/file.png"));
      icon->setAttribute(_("dimension"), _("120x120"));
      icon->setAttribute(_("depth"), _("24"));
      icon->setAttribute(_("mime-type"), _("image/png"));
      icon->setAttribute(_("url"), _("/content/icons/mt-120.png"));
      icons->appendElementChild(icon);

      iconConf->appendElementChild(icons);
      return iconConf;
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