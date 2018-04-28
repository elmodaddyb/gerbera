#include <streaming/shoutcast_options.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;

class ShoutcastOptionsTest : public ::testing::Test {

 public:

  ShoutcastOptionsTest() {};

  virtual ~ShoutcastOptionsTest() {};

  virtual void SetUp() {
    std::string baseUrl = "http://localhost";
    std::string devId = "ABC123";
    bool enabled = true;

    subject = new ShoutcastOptions(baseUrl, devId, enabled);
  }

  virtual void TearDown() {
    delete subject;
  }

  ShoutcastOptions *subject;
};

TEST_F(ShoutcastOptionsTest, CreatesOptionsWithDevIdUrlAndEnabled) {
  EXPECT_STREQ(subject->getBaseUrl().c_str(), "http://localhost");
  EXPECT_STREQ(subject->getDevId().c_str(), "ABC123");
  ASSERT_TRUE(subject->isEnabled());
}
