#include <icons/icon_config.h>
#include <mxml/mxml.h>
#include "gtest/gtest.h"

using namespace ::testing;
using namespace std;
using namespace zmm;
using namespace mxml;

class IconConfigTest : public ::testing::Test {

public:
    IconConfigTest() = default;

    virtual ~IconConfigTest() = default;

    virtual void SetUp() {
    }

    virtual void TearDown() {
      subject = nullptr;
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
      icons->appendElementChild(icon);

      iconConf->appendElementChild(icons);
      return iconConf;
    }

    std::unique_ptr<IconConfig> subject;
};

TEST_F(IconConfigTest, IdentifiesIconLoadingType) {
  Ref<Element> config = mockConfig("static-list");
  subject = make_unique<IconConfig>(config);

  icon_loading_type result = subject->getType();

  EXPECT_EQ(result, static_list);
}

TEST_F(IconConfigTest, LoadsStaticListOfIcons) {
  Ref<Element> config = mockConfig("static-list");
  subject = make_unique<IconConfig>(config);

  std::shared_ptr<std::vector<std::unique_ptr<GerberaIcon>>> result = subject->getIcons();
  const auto &icon = subject->getIcons()->at(0);
  EXPECT_STREQ(icon->path().c_str(), "/icon/path/file.png");
  EXPECT_STREQ(icon->dimension().c_str(), "120x120");
  EXPECT_STREQ(icon->depth().c_str(), "24");
  EXPECT_STREQ(icon->mimeType().c_str(), "image/png");
  EXPECT_EQ(result->size(), 1);
}