#include <array>
#include <memory>
#include <fstream>
#include <common.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <server.h>

using namespace ::testing;

class ServerFixture : public Server {
public:
    ServerFixture() {};
    virtual ~ServerFixture() {};
    zmm::Ref<RequestHandler> wrapCreateRequestHandler(char* filename) {
      return createRequestHandler(filename);
    }
};

class ServerHandlerTest : public ::testing::Test {
public:
    ServerHandlerTest() {};
    virtual ~ServerHandlerTest() {};

    virtual void SetUp() {
      subject = std::make_unique<ServerFixture>();
    }

    virtual void TearDown() {
    };

    std::unique_ptr<ServerFixture> subject;
};

TEST_F(ServerHandlerTest, CreatesServerInstance) {
  EXPECT_NE(subject, nullptr);
}

TEST_F(ServerHandlerTest, CreatesRequestHandlerFor_media) {
  std::string filename = "/content/media";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());

  zmm::Ref<RequestHandler> result = subject->wrapCreateRequestHandler(cstr);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}

TEST_F(ServerHandlerTest, CreatesRequestHandlerFor_interface) {
  std::string filename = "/content/interface?req_type=items";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());

  zmm::Ref<RequestHandler> result = subject->wrapCreateRequestHandler(cstr);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}

// Needs stateful ConfigManager, first
TEST_F(ServerHandlerTest, DISABLED_CreatesRequestHandlerFor_serve) {
  std::string filename = "/content/serve";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());

  zmm::Ref<RequestHandler> result = subject->wrapCreateRequestHandler(cstr);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}

TEST_F(ServerHandlerTest, CreatesRequestHandlerFor_online) {
  std::string filename = "/content/online";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());

  zmm::Ref<RequestHandler> result = subject->wrapCreateRequestHandler(cstr);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}

TEST_F(ServerHandlerTest, CreatesRequestHandlerFor_icons) {
  std::string filename = "/content/icons";
  char *cstr = new char[filename.length() + 1];
  strcpy(cstr, filename.c_str());

  zmm::Ref<RequestHandler> result = subject->wrapCreateRequestHandler(cstr);

  EXPECT_NE(result, nullptr);
  delete [] cstr;
}

