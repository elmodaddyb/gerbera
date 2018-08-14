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

    TaskThreadPoolTest() {
      numCores = 0;
    };

    ~TaskThreadPoolTest() override = default;

    void SetUp() override {
      subject = std::make_unique<TaskThreadPool>();
      numCores = std::thread::hardware_concurrency();
    }

    void TearDown() override {
      subject = nullptr;
    }

    std::unique_ptr<TaskThreadPool> subject;
    unsigned int numCores;
};

TEST_F(TaskThreadPoolTest, InitializesQueueToEmpty) {
  EXPECT_EQ(subject->getQueueSize(), 0);
}

TEST_F(TaskThreadPoolTest, EnqueuesATaskAndTracksItReceived) {
  subject->start(numCores);

  for(int i = 0; i < 5; i++) {
    std::shared_ptr<TaskMock> task(new ::testing::NiceMock<TaskMock>());
    EXPECT_CALL(*task, run()).WillOnce(Return());
    subject->enqueue(task);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(subject->tasksReceived(), 5);
}

TEST_F(TaskThreadPoolTest, EnqueuesATaskAndTracksItCompleted) {
  subject->start(numCores);

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

TEST_F(TaskThreadPoolTest, ProvidesStartMethodToInitializePool) {
  EXPECT_EQ(subject->getPoolSize(), 0);
  unsigned int numCores = std::thread::hardware_concurrency();

  subject->start(numCores);

  EXPECT_EQ(subject->getPoolSize(), numCores);
}

TEST_F(TaskThreadPoolTest, ProvidesStopMethodToStopPool) {
  subject->start(numCores);

  subject->stop();
  EXPECT_EQ(subject->getPoolSize(), 0);
}

TEST_F(TaskThreadPoolTest, ReportsFalseShutdownWhenPoolStarts) {
  subject->start(numCores);

  bool result = subject->isShutdown();

  EXPECT_EQ(result, false);
}

TEST_F(TaskThreadPoolTest, OnlyStartsThreadsOneTime) {
  subject->start(numCores);
  subject->start(numCores);

  long long result = subject->getPoolSize();

  EXPECT_EQ(result, numCores);
}

TEST_F(TaskThreadPoolTest, ReportsTrueShutdownWhenPoolStopped) {
  subject->start(numCores);
  subject->stop();

  bool result = subject->isShutdown();

  EXPECT_EQ(result, true);
}