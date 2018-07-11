#include <task/task_threadpool.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <functional>

using namespace ::testing;

class TaskMock : public Task {
public:
    MOCK_METHOD0(run, void());
};

class TaskThreadPoolTest : public ::testing::Test {

public:

    TaskThreadPoolTest() {};

    virtual ~TaskThreadPoolTest() {};

    virtual void SetUp() {
      subject = std::make_unique<TaskThreadPool>();
    }

    virtual void TearDown() {
      subject = nullptr;
    }

    std::unique_ptr<TaskThreadPool> subject;
    static int count;
};


TEST_F(TaskThreadPoolTest, InitializesThreadPoolBasedOnCores) {
  int numCores = std::thread::hardware_concurrency();

  EXPECT_EQ(subject->getPoolSize(), numCores);
}

TEST_F(TaskThreadPoolTest, InitializesQueueToEmpty) {
  EXPECT_EQ(subject->getQueueSize(), 0);
}

TEST_F(TaskThreadPoolTest, EnqueuesATaskAndTracksItReceived) {
  for(int i = 0; i < 5; i++) {
    std::shared_ptr<TaskMock> task(new ::testing::NiceMock<TaskMock>());
    EXPECT_CALL(*task, run()).WillOnce(Return());
    subject->enqueue(task);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(subject->tasksReceived(), 5);
}

TEST_F(TaskThreadPoolTest, EnqueuesATaskAndTracksItCompleted) {
  for(int i = 0; i < 5; i++) {
    std::shared_ptr<TaskMock> task(new ::testing::NiceMock<TaskMock>());
    EXPECT_CALL(*task, run()).WillOnce(Return());
    subject->enqueue(task);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(subject->tasksCompleted(), 5);
}

TEST_F(TaskThreadPoolTest, InitializesProcessedToZero) {
  EXPECT_EQ(subject->tasksCompleted(), 0);
}