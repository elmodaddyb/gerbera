#include <common.h>
#include <cds_objects.h>
#include <upnp_xml.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace ::testing;
using namespace zmm;
using namespace mxml;

#include <config_manager.h>
class ConfigManagerMock: public ConfigManager {
public:
  MOCK_METHOD1(getBoolOption, bool(config_option_t option));
  MOCK_METHOD1(getOption, zmm::String(config_option_t option));
  MOCK_METHOD0(getIconConfig, std::shared_ptr<IconConfig>());
};

class UpnpXmlTest : public ::testing::Test {

 public:
  UpnpXmlTest() {};
  virtual ~UpnpXmlTest() {};

  virtual void SetUp() {
    zmm::String virtualDir = "/dir/virtual";
    configManager = new ::testing::NiceMock<ConfigManagerMock>();
    subject = new UpnpXMLBuilder(virtualDir);
  }

  virtual void TearDown() {
    configManager = nullptr;
    delete subject;
  };

  zmm::Ref<Element> mockConfig(std::string loadingType) {
    zmm::Ref<Element> iconConf(new Element(_("icon-config")));
    iconConf->setAttribute("type", loadingType);
    zmm::Ref<Element> icons(new Element(_("icons")));

    zmm::Ref<Element> icon(new Element(_("icon")));
    icon->setText(_("/icon/path/file.png"));
    icon->setAttribute(_("resolution"), _("120x120"));
    icon->setAttribute(_("depth"), _("24"));
    icon->setAttribute(_("mime-type"), _("image/png"));
    icon->setAttribute(_("url"), _("/content/icons/mt-120.png"));
    icons->appendElementChild(icon);

    iconConf->appendElementChild(icons);
    return iconConf;
  }

  ConfigManagerMock* configManager;
  UpnpXMLBuilder *subject;
};

TEST_F(UpnpXmlTest, CreatesUpnpDateElement) {
  zmm::Ref<mxml::Element> result = subject->renderAlbumDate(_("2001-01-01"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "2001-01-01");
  EXPECT_STREQ(result->getName().c_str(), "upnp:date");
}

TEST_F(UpnpXmlTest, CreatesUpnpOrchestraElement) {
  zmm::Ref<mxml::Element> result = subject->renderOrchestra(_("Orchestra"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "Orchestra");
  EXPECT_STREQ(result->getName().c_str(), "upnp:orchestra");
}

TEST_F(UpnpXmlTest, CreatesUpnpConductorElement) {
  zmm::Ref<mxml::Element> result = subject->renderConductor(_("Conductor"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "Conductor");
  EXPECT_STREQ(result->getName().c_str(), "upnp:Conductor");
}

TEST_F(UpnpXmlTest, CreatesUpnpAlbumArtUriElement) {
  zmm::Ref<mxml::Element> result = subject->renderAlbumArtURI(_("/some/uri"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "/some/uri");
  EXPECT_STREQ(result->getName().c_str(), "upnp:albumArtURI");
}

TEST_F(UpnpXmlTest, CreatesDcCreatorElement) {
  zmm::Ref<mxml::Element> result = subject->renderCreator(_("Creator"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "Creator");
  EXPECT_STREQ(result->getName().c_str(), "dc:creator");
}

TEST_F(UpnpXmlTest, CreatesSecCaptionInfoElement) {
  zmm::Ref<mxml::Element> result = subject->renderCaptionInfo(_("file.srt"));

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getText().c_str(), "file.srt");
  EXPECT_STREQ(result->getName().c_str(), "sec:CaptionInfoEx");
  EXPECT_STREQ(result->getAttribute("sec:type").c_str(), "srt");
}

TEST_F(UpnpXmlTest, CreatesEventPropertySet) {
  zmm::Ref<mxml::Element> result = subject->createEventPropertySet();

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getName().c_str(), "e:propertyset");
  EXPECT_STREQ(result->getAttribute("xmlns:e").c_str(), "urn:schemas-upnp-org:event-1-0");
  EXPECT_NE(result->getChildByName("e:property"), nullptr);
}

TEST_F(UpnpXmlTest, UpdatesObjectActiveItem) {
  zmm::Ref<CdsObject> obj(new CdsActiveItem());
  std::ostringstream inputXml;
  inputXml << "<item>";  // this is not valid UPNP, but just enough to test with
  inputXml << "<dc:title>Title</dc:title>";
  inputXml << "<dc:description>description</dc:description>";
  inputXml << "<location>/location</location>";
  inputXml << "<mime-type>audio/mpeg</mime-type>";
  inputXml << "<action>action</action>";
  inputXml << "<state>state</state>";
  inputXml << "</item>";

  subject->updateObject(obj, inputXml.str());

  zmm::Ref<CdsActiveItem> aitem = RefCast(obj, CdsActiveItem);
  EXPECT_NE(aitem, nullptr);
  EXPECT_STREQ(aitem->getTitle().c_str(), "Title");
  EXPECT_STREQ(aitem->getMetadata("dc:description").c_str(), "description");
  EXPECT_STREQ(aitem->getLocation().c_str(), "/location");
  EXPECT_STREQ(aitem->getMimeType().c_str(), "audio/mpeg");
  EXPECT_STREQ(aitem->getAction().c_str(), "action");
  EXPECT_STREQ(aitem->getState().c_str(), "state");
}

TEST_F(UpnpXmlTest, CreateResponse) {
  zmm::String actionName = "action";
  zmm::String serviceType = "urn:schemas-upnp-org:service:ContentDirectory:1";

  zmm::Ref<mxml::Element> result = subject->createResponse(actionName, serviceType);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result->getName().c_str(), "u:actionResponse");
  EXPECT_STREQ(result->getAttribute("xmlns:u").c_str(), "urn:schemas-upnp-org:service:ContentDirectory:1");
}

TEST_F(UpnpXmlTest, FirstResourceRendersPureWhenExternalUrl) {
  zmm::Ref<CdsObject> obj(new CdsItemExternalURL());
  obj->setLocation(_("http://localhost/external/url"));

  zmm::Ref<CdsItem> item = RefCast(obj, CdsItem);

  zmm::String result = subject->getFirstResourcePath(item);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result.c_str(), "http://localhost/external/url");
}

TEST_F(UpnpXmlTest, FirstResourceAddsLocalResourceIdToExternalUrlWhenOnlineWithProxy) {
  zmm::Ref<CdsObject> obj(new CdsItemExternalURL());
  obj->setLocation(_("http://localhost/external/url"));
  obj->setID(12345);
  obj->setFlag(OBJECT_FLAG_ONLINE_SERVICE);
  obj->setFlag(OBJECT_FLAG_PROXY_URL);

  zmm::Ref<CdsItem> item = RefCast(obj, CdsItem);

  zmm::String result = subject->getFirstResourcePath(item);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result.c_str(), "content/online/object_id/12345/res_id/0");
}

TEST_F(UpnpXmlTest, FirstResourceAddsLocalResourceIdToItem) {
  zmm::Ref<CdsObject> obj(new CdsItem());
  obj->setLocation(_("local/content"));
  obj->setID(12345);

  zmm::Ref<CdsItem> item = RefCast(obj, CdsItem);

  zmm::String result = subject->getFirstResourcePath(item);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result.c_str(), "content/media/object_id/12345/res_id/0");
}

TEST_F(UpnpXmlTest, FirstResourceAddsContentServeToInternalUrlItem) {
  zmm::Ref<CdsObject> obj(new CdsItemInternalURL());
  obj->setLocation(_("local/content"));
  obj->setID(12345);

  zmm::Ref<CdsItem> item = RefCast(obj, CdsItem);

  zmm::String result = subject->getFirstResourcePath(item);

  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result.c_str(), "/serve/local/content");
}

TEST_F(UpnpXmlTest, RenderDeviceDescriptionWithDefaultIcons) {
  std::string presentationUrl = "http://localhost/presentation";

  EXPECT_CALL(*configManager, getBoolOption(Eq(CFG_SERVER_EXTEND_PROTOCOLINFO))).WillOnce(Return(true));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_NAME))).WillOnce(Return("Gerbera Media Server"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MANUFACTURER_URL))).WillOnce(Return("http://localhost"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_DESCRIPTION))).WillOnce(Return("model description"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_NAME))).WillOnce(Return("model name"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_NUMBER))).WillOnce(Return("1234567890"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_SERIAL_NUMBER))).WillOnce(Return("ABC123"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_UDN))).WillOnce(Return("udn:1234567890"));
  EXPECT_CALL(*configManager, getIconConfig()).WillOnce(Return(nullptr));

  zmm::Ref<ConfigManager> c(configManager);
  zmm::Ref<mxml::Element> result = subject->renderDeviceDescription(presentationUrl, c);

  EXPECT_NE(result, nullptr);
  zmm::Ref<mxml::Element> device = result->getChildByName("device");
  zmm::Ref<mxml::Element> iconList = device->getChildByName("iconList");
  EXPECT_EQ(iconList->elementChildCount(), 9);
}

TEST_F(UpnpXmlTest, RenderDeviceDescriptionWithCustomIcons) {
  std::string presentationUrl = "http://localhost/presentation";
  std::shared_ptr<IconConfig> iconConfig = std::make_shared<IconConfig>(mockConfig("static-list"));

  EXPECT_CALL(*configManager, getBoolOption(Eq(CFG_SERVER_EXTEND_PROTOCOLINFO))).WillOnce(Return(true));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_NAME))).WillOnce(Return("Gerbera Media Server"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MANUFACTURER_URL))).WillOnce(Return("http://localhost"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_DESCRIPTION))).WillOnce(Return("model description"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_NAME))).WillOnce(Return("model name"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_MODEL_NUMBER))).WillOnce(Return("1234567890"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_SERIAL_NUMBER))).WillOnce(Return("ABC123"));
  EXPECT_CALL(*configManager, getOption(Eq(CFG_SERVER_UDN))).WillOnce(Return("udn:1234567890"));
  EXPECT_CALL(*configManager, getIconConfig()).WillOnce(Return(iconConfig));

  zmm::Ref<ConfigManager> c(configManager);
  zmm::Ref<mxml::Element> result = subject->renderDeviceDescription(presentationUrl, c);

  EXPECT_NE(result, nullptr);
  zmm::Ref<mxml::Element> device = result->getChildByName("device");
  zmm::Ref<mxml::Element> iconList = device->getChildByName("iconList");
  EXPECT_EQ(iconList->elementChildCount(), 1);
}