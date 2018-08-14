#include <array>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#ifdef BSD_NATIVE_UUID
#include <uuid.h>
#else
#include <uuid/uuid.h>
#endif
#include <fstream>
#include <ftw.h>

#include <config_manager.h>
#include <config/config_generator.h>
#include <streaming/streaming_options.h>

using namespace zmm;
using namespace mxml;

static int rmFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb) {
  if(remove(pathname) < 0) {
    perror("ERROR: remove");
    return -1;
  }
  return 0;
}

class ConfigManagerTest : public ::testing::Test {

 public:

  ConfigManagerTest() {};

  virtual ~ConfigManagerTest() {};

  virtual void SetUp() {
    gerberaDir = createTempPath();

    std::string grbWeb = gerberaDir + DIR_SEPARATOR + "web";
    create_directory(grbWeb);

    std::string grbJs = gerberaDir + DIR_SEPARATOR + "js";
    create_directory(grbJs);

    std::string configDir = gerberaDir + DIR_SEPARATOR + ".config";
    create_directory(configDir);

    // Create mock files, allowing for .init()
    std::string mockFiles[3] = {
        grbJs + DIR_SEPARATOR + "common.js",
        grbJs + DIR_SEPARATOR + "import.js",
        grbJs + DIR_SEPARATOR + "playlists.js"
    };
    std::ofstream file;
    for (int i = 0; i < 3; i++) {
      file.open(mockFiles[i]);
      file.close();
    }

    config_file = _(configDir.c_str()) + DIR_SEPARATOR + "config.xml";
    home = gerberaDir;
    prefix = gerberaDir;
    magic = "";
    confdir = ".config";

    // Create config using generator
    std::string cfgContent = createConfig();
    file.open(configDir + DIR_SEPARATOR + "config.xml");
    file << cfgContent;
    file.close();

    subject = new ConfigManager();
  };

  std::string createTempPath() {
    uuid_t uuid;
#ifdef BSD_NATIVE_UUID
    char *uuid_str;
    uint32_t status;
    uuid_create(&uuid, &status);
    uuid_to_string(&uuid, &uuid_str, &status);
#else
    char uuid_str[37];
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
#endif
#ifdef BSD_NATIVE_UUID
    free(uuid_str);
#endif

    std::stringstream ss;
    ss << CMAKE_BINARY_DIR << DIR_SEPARATOR << "test" << DIR_SEPARATOR << "test_config" << DIR_SEPARATOR << uuid_str;

    create_directory(ss.str());
    return ss.str();
  }

  void create_directory(std::string dir) {
    if (mkdir(dir.c_str(), 0777) < 0) {
      throw std::runtime_error("Failed to create test_config temporary directory for testing");
    };
  }

  std::string createConfig() {
    ConfigGenerator configGenerator;
    return configGenerator.generate(std::string(home.c_str()), std::string(confdir.c_str()), std::string(prefix.c_str()), magic);
  }

  virtual void TearDown() {
    // cleanup temporary directory
    if (nftw(gerberaDir.c_str(), rmFiles,10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0) {
      perror("ERROR: ntfw");
      exit(1);
    }
    delete subject;
  };

  std::string gerberaDir;
  String config_file;
  std::string home;
  std::string confdir;
  std::string prefix;
  std::string magic;
  ConfigManager *subject;
};

TEST_F(ConfigManagerTest, LoadsWebUIDefaultValues) {
  subject->setStaticArgs(config_file, _(home.c_str()), _(confdir.c_str()), _(prefix.c_str()), _(magic.c_str()));

  subject->init();

  ASSERT_TRUE(subject->getBoolOption(CFG_SERVER_UI_ENABLED));
  ASSERT_TRUE(subject->getBoolOption(CFG_SERVER_UI_SHOW_TOOLTIPS));
  ASSERT_FALSE(subject->getBoolOption(CFG_SERVER_UI_ACCOUNTS_ENABLED));
  ASSERT_EQ(30, subject->getIntOption(CFG_SERVER_UI_SESSION_TIMEOUT));
}

TEST_F(ConfigManagerTest, ThrowsExceptionWhenMissingConfigFileAndNoDefault) {
  config_file = nullptr;
  std::string notExistsDir = home + DIR_SEPARATOR + "not_exists";
  subject->setStaticArgs(config_file, _(notExistsDir.c_str()), _(confdir.c_str()), _(prefix.c_str()), _(magic.c_str()));

  ASSERT_THROW(subject->init(), zmm::Exception);
}

TEST_F(ConfigManagerTest, LoadsConfigFromDefaultHomeWhenExistsButNotSpecified) {
  config_file = nullptr;
  subject->setStaticArgs(config_file, _(home.c_str()), _(confdir.c_str()), _(prefix.c_str()), _(magic.c_str()));

  subject->init();

  ASSERT_TRUE(subject->getBoolOption(CFG_SERVER_UI_ENABLED));
  ASSERT_TRUE(subject->getBoolOption(CFG_SERVER_UI_SHOW_TOOLTIPS));
  ASSERT_FALSE(subject->getBoolOption(CFG_SERVER_UI_ACCOUNTS_ENABLED));
  ASSERT_EQ(30, subject->getIntOption(CFG_SERVER_UI_SESSION_TIMEOUT));
}

TEST_F(ConfigManagerTest, LoadsDefaultStreamingOptions) {
  subject->setStaticArgs(config_file, _(home.c_str()), _(confdir.c_str()), _(prefix.c_str()), _(magic.c_str()));

  subject->init();

  auto streamingOptions = subject->getStreamingOptions();
  auto shoutcastOption = streamingOptions->shoutcastOptions();
  auto playlists = streamingOptions->playlists();

  ASSERT_NE(streamingOptions, nullptr);
  ASSERT_NE(shoutcastOption, nullptr);
  EXPECT_STREQ(shoutcastOption->getBaseUrl().c_str(), "http://api.shoutcast.com");
  EXPECT_STREQ(shoutcastOption->getDevId().c_str(), "");
  ASSERT_FALSE(shoutcastOption->isEnabled());
  ASSERT_NE(playlists, nullptr);
  EXPECT_EQ(0, playlists->getSize());
}

